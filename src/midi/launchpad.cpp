// Launchpad programmer-mode driver.
//
// Talks to a Launchpad Mini Mk3 / X / Pro Mk3 over USB host MIDI.
// Programmer mode bypasses session logic so we can address every pad
// and side button by raw note number / palette index.
//
// Phase 1g: implementation lays out the API and wires send/receive against
// the UsbHost transport. Tested against real hardware comes when boards are
// available.

#include "picoadk/midi/launchpad.h"
#include "picoadk/hal/usb_host.h"
#include "picoadk/hal/midi.h"

#include <cstring>

namespace picoadk::launchpad {

namespace {

constexpr uint8_t kSysexHeader[] = { 0xF0, 0x00, 0x20, 0x29, 0x02 };
//                                          \____/  \____/  \__/
//                                           Novation Mini/X/Pro

uint8_t model_byte(Model m) {
    switch (m) {
        case Model::MiniMk3: return 0x0D;
        case Model::X:       return 0x0C;
        case Model::ProMk3:  return 0x0E;
        default:             return 0x00;
    }
}

void send_sysex(const uint8_t* body, std::size_t len) {
    uint8_t buf[64];
    std::size_t n = 0;
    memcpy(buf + n, kSysexHeader, sizeof(kSysexHeader));
    n += sizeof(kSysexHeader);
    memcpy(buf + n, body, len);
    n += len;
    buf[n++] = 0xF7;
    Midi::send_raw(buf, n, MidiTransport::UsbHost);
}

bool   g_connected = false;
Model  g_model     = Model::Unknown;

// Smart update: only push pads that actually changed since the last call.
// `g_pad_state` is the *desired* state; `g_pad_last` mirrors what we last
// transmitted. `present()` walks the diff and emits one sysex bulk frame
// when more than 8 pads changed (cheaper than 8× note-on), otherwise
// per-pad note-on messages.
struct PadState {
    uint8_t palette;
    uint8_t r, g, b;
    bool    rgb_mode;
    LedMode mode;
};
PadState g_pad_state[8][8] = {};
PadState g_pad_last [8][8] = {};
bool     g_pad_dirty[8][8] = {};

}  // namespace

bool Driver::connect() {
    if (!UsbHost::any_midi_device_connected()) return false;
    g_model     = Model::MiniMk3;          // TODO: device inquiry to detect properly
    g_connected = true;
    // Enter programmer mode (0x0E 0x01).
    uint8_t body[] = { model_byte(g_model), 0x0E, 0x01 };
    send_sysex(body, sizeof(body));
    return true;
}

void Driver::disconnect() {
    if (!g_connected) return;
    uint8_t body[] = { model_byte(g_model), 0x0E, 0x00 };
    send_sysex(body, sizeof(body));
    g_connected = false;
}

Model Driver::model() const noexcept     { return g_model; }
bool  Driver::connected() const noexcept { return g_connected; }

void Driver::set_pad_index(uint8_t pad_index, uint8_t palette) {
    if (pad_index < 11 || pad_index > 88) return;
    uint8_t x = (uint8_t)((pad_index % 10) - 1);
    uint8_t y = (uint8_t)((pad_index / 10) - 1);
    if (x >= 8 || y >= 8) return;
    set_pad(x, y, palette);
}
void Driver::set_pad(uint8_t x, uint8_t y, uint8_t palette) {
    if (x >= 8 || y >= 8) return;
    auto& s = g_pad_state[y][x];
    if (!s.rgb_mode && s.palette == palette && s.mode == LedMode::Static) return;
    s = { palette, 0, 0, 0, false, LedMode::Static };
    g_pad_dirty[y][x] = true;
}
void Driver::set_pad_rgb(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= 8 || y >= 8) return;
    auto& s = g_pad_state[y][x];
    if (s.rgb_mode && s.r == r && s.g == g && s.b == b) return;
    s = { 0, r, g, b, true, LedMode::Static };
    g_pad_dirty[y][x] = true;
}
void Driver::set_pad_mode(uint8_t x, uint8_t y, LedMode mode) {
    if (x >= 8 || y >= 8) return;
    auto& s = g_pad_state[y][x];
    if (s.mode == mode) return;
    s.mode = mode;
    g_pad_dirty[y][x] = true;
}
void Driver::set_button(ButtonId id, uint8_t palette) {
    uint8_t b[3] = { 0xB0, (uint8_t)id, palette };
    Midi::send_raw(b, 3, MidiTransport::UsbHost);
}
void Driver::blit_grid_palette(const uint8_t (&grid)[8][8]) {
    for (uint8_t y = 0; y < 8; ++y)
        for (uint8_t x = 0; x < 8; ++x)
            set_pad(x, y, grid[y][x]);
}
void Driver::blit_grid_rgb(const uint8_t (&rgb)[8][8][3]) {
    for (uint8_t y = 0; y < 8; ++y)
        for (uint8_t x = 0; x < 8; ++x)
            set_pad_rgb(x, y, rgb[y][x][0], rgb[y][x][1], rgb[y][x][2]);
}
void Driver::clear() {
    for (uint8_t y = 0; y < 8; ++y)
        for (uint8_t x = 0; x < 8; ++x)
            set_pad(x, y, 0);
}

// Flush diffs to the panel. Called explicitly OR from process(); pad updates
// stay buffered until then so a full-grid redraw becomes one sysex frame
// instead of 64 individual messages. Below ~8 changes per frame the per-pad
// note-on messages are cheaper, so we pick at runtime.
namespace {
void flush_pad_diffs() {
    if (!g_connected) return;
    uint8_t changed = 0;
    for (uint8_t y = 0; y < 8; ++y)
        for (uint8_t x = 0; x < 8; ++x)
            if (g_pad_dirty[y][x]) ++changed;
    if (!changed) return;

    bool any_rgb = false;
    for (uint8_t y = 0; y < 8 && !any_rgb; ++y)
        for (uint8_t x = 0; x < 8 && !any_rgb; ++x)
            if (g_pad_dirty[y][x] && g_pad_state[y][x].rgb_mode) any_rgb = true;

    if (any_rgb || changed >= 8) {
        // Bulk sysex frame: <model> 03 <spec_per_pad...>
        uint8_t body[6 + 64 * 5];
        std::size_t n = 0;
        body[n++] = model_byte(g_model);
        body[n++] = 0x03;     // "LED specification" command
        for (uint8_t y = 0; y < 8; ++y) {
            for (uint8_t x = 0; x < 8; ++x) {
                if (!g_pad_dirty[y][x]) continue;
                uint8_t pad = (uint8_t)((y + 1) * 10 + (x + 1));
                const auto& s = g_pad_state[y][x];
                if (s.rgb_mode) {
                    body[n++] = 0x03; body[n++] = pad;
                    body[n++] = (uint8_t)(s.r >> 1);
                    body[n++] = (uint8_t)(s.g >> 1);
                    body[n++] = (uint8_t)(s.b >> 1);
                } else {
                    uint8_t spec = (s.mode == LedMode::Static) ? 0x00
                                 : (s.mode == LedMode::Flash)  ? 0x01
                                 :                                0x02;
                    body[n++] = spec; body[n++] = pad; body[n++] = s.palette;
                }
            }
        }
        send_sysex(body, n);
    } else {
        for (uint8_t y = 0; y < 8; ++y) {
            for (uint8_t x = 0; x < 8; ++x) {
                if (!g_pad_dirty[y][x]) continue;
                const auto& s = g_pad_state[y][x];
                uint8_t pad = (uint8_t)((y + 1) * 10 + (x + 1));
                uint8_t status = (s.mode == LedMode::Static) ? 0x90
                              :  (s.mode == LedMode::Flash)  ? 0x91
                              :                                0x92;
                uint8_t b[3] = { status, pad, s.palette };
                Midi::send_raw(b, 3, MidiTransport::UsbHost);
            }
        }
    }
    for (uint8_t y = 0; y < 8; ++y)
        for (uint8_t x = 0; x < 8; ++x)
            if (g_pad_dirty[y][x]) {
                g_pad_last[y][x]  = g_pad_state[y][x];
                g_pad_dirty[y][x] = false;
            }
}
}  // namespace

void Driver::process() {
    flush_pad_diffs();
    // Inbound pad events are delivered as note-on (vel>0) / note-on (vel=0)
    // by the Launchpad. Translation from MIDI notes to grid coords happens
    // here so user code only sees (x, y, vel).
    //
    // The actual incoming MIDI is pumped by Midi::process(); we install a
    // raw callback once and decode every relevant message.
    static bool installed = false;
    static Driver* self_ptr = nullptr;
    self_ptr = this;
    if (!installed) {
        installed = true;
        Midi::set_raw([](const MidiMessage& m) {
            if (m.source != MidiTransport::UsbHost || !self_ptr) return;
            uint8_t status = m.status & 0xF0;
            if (status == 0x90 || status == 0x80) {
                uint8_t note = m.data1;
                uint8_t vel  = m.data2;
                if (note >= 11 && note <= 88) {
                    uint8_t x = (uint8_t)((note % 10) - 1);
                    uint8_t y = (uint8_t)((note / 10) - 1);
                    if (x < 8 && y < 8) {
                        if (status == 0x90 && vel) {
                            if (self_ptr->on_pad_pressed)  self_ptr->on_pad_pressed(x, y, vel);
                        } else {
                            if (self_ptr->on_pad_released) self_ptr->on_pad_released(x, y, vel);
                        }
                    }
                }
            } else if (status == 0xB0) {
                if (self_ptr->on_button)
                    self_ptr->on_button(static_cast<ButtonId>(m.data1), m.data2 > 0);
            }
        });
    }
}

}  // namespace picoadk::launchpad
