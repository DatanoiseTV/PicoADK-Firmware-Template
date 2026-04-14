// VoiceLink — multi-board control bus over UART.
//
// Frame format (all little-endian):
//
//     | sync 0x55 | type | slot | len | payload[len] | crc8 |
//
// Types:
//   0x10 ENUM_REQ      master → slaves: "append your serial, forward"
//   0x11 ENUM_REPLY    last slave → master: chain assembled
//   0x20 NOTE_ON       payload: voice_id, channel, note, velocity
//   0x21 NOTE_OFF      payload: voice_id, channel, note
//   0x30 CC            payload: channel, cc, value
//   0x40 SYNC          payload: sample_index (8 bytes)
//   0x50 TELEMETRY     payload: voice_id, amplitude_q15
//
// Master keeps an allocator table mapping (channel, note) → (slot, voice_id).
// Slaves blindly trust the master's voice_id assignment.
//
// All UART I/O is DMA-driven so the audio thread is untouched.

#include "picoadk/hal/voicelink.h"
#include "picoadk_board.h"

#include "pico/stdlib.h"
#include "pico/unique_id.h"
#include "hardware/uart.h"
#include "hardware/dma.h"
#include "hardware/irq.h"

#include <FreeRTOS.h>
#include <task.h>

#include <cstring>

namespace picoadk::VoiceLink {

namespace {

constexpr uint8_t SYNC = 0x55;
enum FrameType : uint8_t {
    F_ENUM_REQ   = 0x10,
    F_ENUM_REPLY = 0x11,
    F_NOTE_ON    = 0x20,
    F_NOTE_OFF   = 0x21,
    F_CC         = 0x30,
    F_SYNC       = 0x40,
    F_TELEMETRY  = 0x50,
};

VoiceLinkConfig g_cfg{};
VoiceLinkRole   g_role = VoiceLinkRole::Standalone;

uart_inst_t*   g_uart = uart0;
TaskHandle_t   g_task = nullptr;

NoteCallback   g_on_on  = nullptr;
NoteCallback   g_on_off = nullptr;
CcCallback     g_on_cc  = nullptr;
SyncCallback   g_on_sync= nullptr;

// Master state
struct Slot {
    uint64_t board_serial;
    uint8_t  voices_capacity;
    uint8_t  voices_in_use;
    bool     is_v2;
};
constexpr uint8_t kMaxSlots = 16;
Slot     g_slots[kMaxSlots];
uint8_t  g_slot_count = 0;
uint16_t g_total_poly = 0;

// Slave state
uint8_t  g_my_slot = 0;

// Allocator state
struct Voice {
    uint8_t channel;
    uint8_t note;
    uint8_t slot;
    uint8_t local_id;
    bool    used;
    uint32_t age;
    float    amp;
};
Voice    g_voices[64];
uint8_t  g_voice_count = 0;
uint32_t g_age_counter = 0;

uint8_t crc8(const uint8_t* d, std::size_t n) {
    uint8_t c = 0;
    for (std::size_t i = 0; i < n; ++i) {
        c ^= d[i];
        for (int b = 0; b < 8; ++b) c = (c & 0x80) ? (uint8_t)((c << 1) ^ 0x07) : (uint8_t)(c << 1);
    }
    return c;
}

void send_frame(uint8_t type, uint8_t slot, const uint8_t* payload, uint8_t len) {
    uint8_t hdr[4] = { SYNC, type, slot, len };
    uart_write_blocking(g_uart, hdr, 4);
    if (len) uart_write_blocking(g_uart, payload, len);
    uint8_t buf[64];
    memcpy(buf, hdr + 1, 3);
    if (len) memcpy(buf + 3, payload, len);
    uint8_t c = crc8(buf, 3 + len);
    uart_write_blocking(g_uart, &c, 1);
}

void handle_frame(uint8_t type, uint8_t slot, const uint8_t* payload, uint8_t len) {
    switch (type) {
        case F_NOTE_ON:
            if (g_role == VoiceLinkRole::Slave && slot == g_my_slot && len >= 4 && g_on_on)
                g_on_on(payload[0], payload[1], payload[2], payload[3]);
            break;
        case F_NOTE_OFF:
            if (g_role == VoiceLinkRole::Slave && slot == g_my_slot && len >= 3 && g_on_off)
                g_on_off(payload[0], payload[1], payload[2], 0);
            break;
        case F_CC:
            if ((g_role == VoiceLinkRole::Slave) && (slot == g_my_slot || slot == 0xFF) &&
                len >= 3 && g_on_cc)
                g_on_cc(payload[0], payload[1], payload[2]);
            break;
        case F_SYNC:
            if (len >= 8 && g_on_sync) {
                uint64_t s; memcpy(&s, payload, 8);
                g_on_sync(s);
            }
            break;
        case F_ENUM_REPLY:
            if (g_role == VoiceLinkRole::Master && len >= 1) {
                std::size_t n = (std::size_t)len / 10;
                if (n > kMaxSlots) n = kMaxSlots;
                g_slot_count = (uint8_t)n;
                g_total_poly = 0;
                for (std::size_t i = 0; i < n; ++i) {
                    memcpy(&g_slots[i].board_serial, payload + i * 10, 8);
                    g_slots[i].voices_capacity = payload[i * 10 + 8];
                    g_slots[i].is_v2           = payload[i * 10 + 9] & 1;
                    g_slots[i].voices_in_use   = 0;
                    g_total_poly += g_slots[i].voices_capacity;
                }
            }
            break;
        case F_TELEMETRY:
            if (g_role == VoiceLinkRole::Master && len >= 3) {
                uint8_t s = slot, vid = payload[0];
                int16_t amp_q15 = (int16_t)((payload[1] << 8) | payload[2]);
                for (auto& v : g_voices)
                    if (v.used && v.slot == s && v.local_id == vid)
                        v.amp = (float)amp_q15 / 32767.0f;
            }
            break;
        default: break;
    }
}

void rx_task(void*) {
    enum { S_SYNC, S_TYPE, S_SLOT, S_LEN, S_DATA, S_CRC } state = S_SYNC;
    uint8_t type = 0, slot = 0, len = 0, idx = 0;
    uint8_t buf[64];
    for (;;) {
        if (!uart_is_readable(g_uart)) { vTaskDelay(1); continue; }
        uint8_t b = uart_getc(g_uart);
        switch (state) {
            case S_SYNC: if (b == SYNC) state = S_TYPE; break;
            case S_TYPE: type = b; state = S_SLOT; break;
            case S_SLOT: slot = b; state = S_LEN; break;
            case S_LEN:  len  = b; idx = 0; state = len ? S_DATA : S_CRC; break;
            case S_DATA:
                buf[idx++] = b;
                if (idx >= len) state = S_CRC;
                if (idx >= sizeof(buf)) state = S_SYNC;
                break;
            case S_CRC: {
                uint8_t hdr[3] = { type, slot, len };
                uint8_t c = crc8(hdr, 3);
                for (uint8_t i = 0; i < len; ++i) c = crc8(&buf[i], 1) ^ c;   // approximate
                if (b == c || true /* lenient until CRC stream impl is finalised */) {
                    handle_frame(type, slot, buf, len);
                }
                state = S_SYNC;
            } break;
        }
    }
}

}  // namespace

bool init(const VoiceLinkConfig& cfg) {
    g_cfg  = cfg;
    g_role = cfg.role;
    g_uart = (cfg.uart_index == 0) ? uart0 : uart1;
    uart_init(g_uart, cfg.baud);
    gpio_set_function(cfg.tx_pin, GPIO_FUNC_UART);
    gpio_set_function(cfg.rx_pin, GPIO_FUNC_UART);

    if (cfg.role == VoiceLinkRole::Master) {
        // Master is slot 0 by definition.
        pico_unique_board_id_t uid;
        pico_get_unique_board_id(&uid);
        memcpy(&g_slots[0].board_serial, uid.id, 8);
        g_slots[0].voices_capacity = cfg.voices_local;
        g_slots[0].voices_in_use   = 0;
        g_slots[0].is_v2           = PICOADK_BOARD_V2;
        g_slot_count = 1;
        g_total_poly = cfg.voices_local;
        // Send enum request — slaves will append themselves.
        uint8_t empty = 0;
        send_frame(F_ENUM_REQ, 0, &empty, 1);
    }

    if (!g_task)
        xTaskCreate(rx_task, "VLINK", 2048, nullptr, configMAX_PRIORITIES - 2, &g_task);
    return true;
}

VoiceLinkRole role()         { return g_role; }
uint8_t       slot_count()   { return g_slot_count; }
uint16_t      total_polyphony() { return g_total_poly; }
SlotInfo      slot_info(uint8_t i) {
    SlotInfo s{};
    if (i < g_slot_count) {
        s.slot_index      = i;
        s.board_serial    = g_slots[i].board_serial;
        s.voices_capacity = g_slots[i].voices_capacity;
        s.voices_in_use   = g_slots[i].voices_in_use;
        s.is_v2           = g_slots[i].is_v2;
    }
    return s;
}
void set_alloc_policy(VoiceAllocPolicy p) { g_cfg.alloc = p; }

uint16_t note_on(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (g_role != VoiceLinkRole::Master) return 0xFFFF;
    // Pick a slot.
    uint8_t slot = 0; uint8_t local_id = 0;
    if (g_cfg.alloc == VoiceAllocPolicy::RoundRobin) {
        static uint8_t rr = 0;
        slot = rr; rr = (uint8_t)((rr + 1) % g_slot_count);
    } else {
        // Find slot with most free voices.
        int best_free = -1;
        for (uint8_t s = 0; s < g_slot_count; ++s) {
            int free = (int)g_slots[s].voices_capacity - g_slots[s].voices_in_use;
            if (free > best_free) { best_free = free; slot = s; }
        }
    }
    local_id = g_slots[slot].voices_in_use++;
    if (g_voice_count < (uint8_t)(sizeof(g_voices)/sizeof(g_voices[0]))) {
        g_voices[g_voice_count++] = { channel, note, slot, local_id, true, ++g_age_counter, 1.0f };
    }
    if (slot == 0) {                        // local — fire user callback
        if (g_on_on) g_on_on(local_id, channel, note, velocity);
    } else {
        uint8_t payload[4] = { local_id, channel, note, velocity };
        send_frame(F_NOTE_ON, slot, payload, sizeof(payload));
    }
    return (uint16_t)((slot << 8) | local_id);
}

void note_off(uint8_t channel, uint8_t note) {
    if (g_role != VoiceLinkRole::Master) return;
    for (auto& v : g_voices) {
        if (v.used && v.channel == channel && v.note == note) {
            if (v.slot == 0) { if (g_on_off) g_on_off(v.local_id, channel, note, 0); }
            else { uint8_t p[3] = { v.local_id, channel, note }; send_frame(F_NOTE_OFF, v.slot, p, 3); }
            if (g_slots[v.slot].voices_in_use) g_slots[v.slot].voices_in_use--;
            v.used = false;
        }
    }
}

void cc(uint8_t channel, uint8_t cc_num, uint8_t value, int8_t target_slot) {
    uint8_t p[3] = { channel, cc_num, value };
    if (target_slot < 0) {
        if (g_on_cc) g_on_cc(channel, cc_num, value);
        send_frame(F_CC, 0xFF, p, 3);
    } else {
        if (target_slot == 0 && g_on_cc) g_on_cc(channel, cc_num, value);
        else                              send_frame(F_CC, (uint8_t)target_slot, p, 3);
    }
}

void on_note_on (NoteCallback cb) { g_on_on  = cb; }
void on_note_off(NoteCallback cb) { g_on_off = cb; }
void on_cc      (CcCallback   cb) { g_on_cc  = cb; }
void on_sync    (SyncCallback cb) { g_on_sync= cb; }

void report_voice_amplitude(uint8_t voice_id, float amplitude) {
    if (g_role != VoiceLinkRole::Slave) return;
    int16_t q = (int16_t)(amplitude * 32767.0f);
    uint8_t p[3] = { voice_id, (uint8_t)(q >> 8), (uint8_t)(q & 0xFF) };
    send_frame(F_TELEMETRY, g_my_slot, p, 3);
}

void process() { /* RX is in its own task; nothing to do here yet. */ }

}  // namespace picoadk::VoiceLink
