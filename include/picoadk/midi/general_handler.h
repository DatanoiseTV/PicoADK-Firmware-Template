// General MIDI handler — high-level dispatch on top of picoadk::Midi.
//
// Aggregates the messy bits MIDI implementations always re-invent:
//   * 14-bit pitch-bend (signed, with configurable bend range in semitones)
//   * RPN/NRPN MSB/LSB pairing, with data entry / increment / decrement
//   * Channel mode messages (all-notes-off, all-sound-off, omni/poly, …)
//   * Channel pressure, polyphonic aftertouch
//   * Program change
//   * MIDI clock + transport (start/stop/continue, 24 ppqn tick)
//
// Install Handler::install(my_handler) once and the Midi callbacks route
// through it before being broken out into per-message callbacks.

#pragma once

#include <cstdint>

#include "picoadk/hal/midi.h"

namespace picoadk::midi {

struct Handler {
    // ---- Notes -------------------------------------------------------------
    void (*on_note_on)        (uint8_t channel, uint8_t note, uint8_t velocity) = nullptr;
    void (*on_note_off)       (uint8_t channel, uint8_t note, uint8_t velocity) = nullptr;
    void (*on_poly_aftertouch)(uint8_t channel, uint8_t note, uint8_t pressure) = nullptr;

    // ---- Continuous controllers -------------------------------------------
    void (*on_cc)             (uint8_t channel, uint8_t cc, uint8_t value) = nullptr;
    void (*on_program_change) (uint8_t channel, uint8_t program) = nullptr;
    void (*on_channel_pressure)(uint8_t channel, uint8_t pressure) = nullptr;

    // 14-bit pitch bend, semitones (negative=down).
    void (*on_pitch_bend)     (uint8_t channel, float semitones) = nullptr;

    // ---- (N)RPN ------------------------------------------------------------
    // Combined MSB/LSB delivered as 14-bit number/value.
    void (*on_rpn)            (uint8_t channel, uint16_t number, uint16_t value) = nullptr;
    void (*on_nrpn)           (uint8_t channel, uint16_t number, uint16_t value) = nullptr;

    // ---- Channel mode -----------------------------------------------------
    void (*on_all_sound_off)  (uint8_t channel) = nullptr;
    void (*on_all_notes_off)  (uint8_t channel) = nullptr;
    void (*on_reset_controllers)(uint8_t channel) = nullptr;

    // ---- System-real-time --------------------------------------------------
    void (*on_clock_tick)     ()                = nullptr;   // 24 ppqn
    void (*on_start)          ()                = nullptr;
    void (*on_stop)           ()                = nullptr;
    void (*on_continue)       ()                = nullptr;
    void (*on_song_position)  (uint16_t mid_beats) = nullptr;

    // Catch-all for anything not broken out (sysex etc).
    void (*on_raw)            (const MidiMessage& m) = nullptr;

    // Filter: only dispatch messages on these channels (bit per channel).
    // 0 = all channels.
    uint16_t channel_mask = 0;

    // Pitch-bend range used to convert the 14-bit value to semitones.
    float    pitch_bend_range_semis = 2.0f;
};

namespace Handler_ns {

// Install a handler. Replaces any previous installation.
void install(Handler* h);

}  // namespace Handler_ns
}  // namespace picoadk::midi
