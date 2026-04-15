// Rotary encoder HAL.
//
// Designed for "lots of encoders, no per-edge interrupt overhead." A single
// PIO state machine reads N encoders by sampling their A/B lines on a rising
// PIO clock, decodes quadrature in PIO assembly, and pushes deltas to a DMA
// ring buffer. The CPU drains the ring at audio-rate cost no matter how many
// encoders are configured.
//
// On v1 the legacy single-encoder path (PIN_ENC_*) still works through this
// API; just declare one encoder.

#pragma once

#include <cstdint>
#include <cstddef>

namespace picoadk {

struct EncoderPins {
    uint8_t pin_a;
    uint8_t pin_b;
    uint8_t pin_btn = 255;     // 255 = no button
    bool    invert  = false;   // swap CW/CCW
    bool    half_step = false; // count every quadrature edge instead of full detent
};

struct EncoderReading {
    int32_t position;          // accumulated detents
    int8_t  delta;             // change since last poll, saturated to int8 range
    bool    button_down;       // current state (debounced)
    bool    button_pressed;    // edge: low->high since last poll
    bool    button_released;   // edge: high->low since last poll
};

namespace Encoders {

// Configure a bank of encoders. `pins` may be on any GPIO; the implementation
// groups consecutive pins onto a single PIO state machine when possible.
// Returns false if more SMs are required than we have available.
bool init(const EncoderPins* pins, std::size_t count);

// Snapshot all encoders. `out` must be at least `count` entries long.
// Cheap (typically <2 µs per encoder); call from a low-priority task at
// 200–1000 Hz, or from your control-rate audio block.
void poll(EncoderReading* out, std::size_t count);

// Reset accumulator(s).
void reset(std::size_t index);

}  // namespace Encoders
}  // namespace picoadk
