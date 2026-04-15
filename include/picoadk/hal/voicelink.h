// VoiceLink — wire several PicoADKs together as one polyphonic instrument.
//
// One master, up to 15 slaves, daisy-chained over a single UART pair
// (TX → RX of next board, with the chain looping back to the master so it
// can detect drops). Default link rate is 3 Mbaud, well within RP2040 / 2350
// UART hardware limits. Framing is DMA-fed — the CPU pays for parsing only,
// never for byte-banging.
//
// Audio always runs locally on every board. VoiceLink only carries control
// data: note events with explicit voice ids, parameter changes broadcast or
// targeted, and a sample-aligned sync clock for things that need to start
// together (LFO reset, sample triggers, …).
//
// Topology auto-discovery:
//   * On boot the master sends an "enumerate" frame.
//   * Each slave appends its serial id and forwards the frame down the chain.
//   * The frame eventually returns to the master, who now knows the chain
//     length, slot order, and any duplicates / missing units.
//
// Voice allocation policies:
//   * RoundRobin — simplest, each voice goes to the next slot.
//   * Oldest    — steal the oldest playing voice when out of slots.
//   * Quietest  — steal the voice with the lowest envelope amplitude (slaves
//                 report this as a periodic telemetry frame).

#pragma once

#include <cstddef>
#include <cstdint>

namespace picoadk {

enum class VoiceLinkRole : uint8_t {
    Standalone = 0,
    Master,
    Slave,
};

enum class VoiceAllocPolicy : uint8_t {
    RoundRobin,
    Oldest,
    Quietest,
};

struct VoiceLinkConfig {
    VoiceLinkRole role        = VoiceLinkRole::Standalone;
    uint8_t       uart_index  = 0;          // 0 → uart0, 1 → uart1
    uint8_t       tx_pin      = 12;
    uint8_t       rx_pin      = 13;
    uint32_t      baud        = 3'000'000;  // 3 Mbaud — RP2040/2350 happy here

    // How many simultaneous voices THIS board is willing to play. Each slave
    // reports this during enumeration so the master can allocate accurately
    // even when the chain is heterogeneous (e.g. one v2 with 16 voices in
    // line with two v1s offering 4 each).
    uint8_t       voices_local = 8;

    VoiceAllocPolicy alloc    = VoiceAllocPolicy::Oldest;
};

struct SlotInfo {
    uint8_t  slot_index;        // 0 = master, 1..N = slaves in chain order
    uint64_t board_serial;      // pico_unique_id-derived
    uint8_t  voices_capacity;   // what that slot reported as its polyphony
    uint8_t  voices_in_use;
    bool     is_v2;             // true if RP2350
};

namespace VoiceLink {

bool init(const VoiceLinkConfig& cfg);

// Tells you what your role ended up being (e.g. Master may demote itself to
// Standalone if no slaves are detected after enumeration).
VoiceLinkRole role();

// Master only — number of slots in the chain including the master (1..16).
uint8_t       slot_count();

// Master only — total polyphony available across the chain (sum of voices_local).
uint16_t      total_polyphony();

// Master only — per-slot info (use slot_count() to bound).
SlotInfo      slot_info(uint8_t index);

// Master only — current allocation policy (mutable at runtime).
void          set_alloc_policy(VoiceAllocPolicy p);

// MASTER side: forward a note to wherever the allocator decides.
//   Returns the assigned (slot, local_voice_id) pair as one packed uint16
//   (slot in high byte, voice id in low byte) — useful for tracking.
uint16_t      note_on (uint8_t channel, uint8_t note, uint8_t velocity);
void          note_off(uint8_t channel, uint8_t note);
void          cc      (uint8_t channel, uint8_t cc, uint8_t value,
                       int8_t target_slot = -1);   // -1 = broadcast

// SLAVE side: callbacks fire when the master tells us to do something.
using NoteCallback = void (*)(uint8_t voice_id, uint8_t channel, uint8_t note, uint8_t velocity);
using CcCallback   = void (*)(uint8_t channel, uint8_t cc, uint8_t value);
using SyncCallback = void (*)(uint64_t sample_index);

void on_note_on (NoteCallback cb);
void on_note_off(NoteCallback cb);
void on_cc      (CcCallback   cb);
void on_sync    (SyncCallback cb);

// Slave-side telemetry: report current envelope amplitude per voice so the
// master's "Quietest" policy has data to work with.
void report_voice_amplitude(uint8_t voice_id, float amplitude);

// Pump background work — call from a low-priority task. Audio-path frames
// are handled in the UART DMA IRQ regardless.
void process();

}  // namespace VoiceLink
}  // namespace picoadk
