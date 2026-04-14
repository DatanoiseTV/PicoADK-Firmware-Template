// Controls HAL — pots, CV inputs, encoders, buttons, gates.
//
// The 8-channel external SPI ADC (MCP3208) on both boards is exposed via
// `read_adc8(ch)`; raw RP2040/RP2350 ADC pins are exposed via
// `read_adc(channel)` and read on demand. Higher-level helpers (encoder
// decoding, debounce, pot smoothing) live here so user code stays trivial.

#pragma once

#include <cstdint>

namespace picoadk {

struct EncoderState {
    int32_t  position;     // accumulated detents
    int8_t   delta;        // change since last poll
    bool     pressed;
};

namespace Controls {

void init();

// External 8-channel SPI ADC (12-bit, 0..4095). Returns 0 on out-of-range.
uint16_t read_adc8(uint8_t channel);

// Internal RP2040/RP2350 ADC on PIN_CV_IN_*/PIN_POT_*. 12-bit value.
uint16_t read_adc(uint8_t channel);

// Smoothed read in [0.0, 1.0].
float    read_adc8_norm(uint8_t channel);
float    read_adc_norm (uint8_t channel);

// Encoder helpers (board encoder on ENC_A/ENC_B/ENC_BTN).
EncoderState read_encoder();
void         reset_encoder();

// Trigger button.
bool read_trig_button();

// Set the chase LEDs (4 bits).
void set_status_leds(uint8_t mask);
void set_note_led(bool on);

}  // namespace Controls
}  // namespace picoadk
