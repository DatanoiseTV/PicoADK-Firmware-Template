// Encoders HAL — quadrature decoding.
//
// Compact polled implementation: each call to `poll()` snapshots all
// configured encoders' A/B GPIO levels and runs a 4-state quadrature decoder
// per encoder. Cheap (a few µs even for 16 encoders) and works on any pin —
// no PIO state machine needed.
//
// (A PIO-based version using picoadk_quad lives in src/hal/pio/quad_decoder.pio
// and is wired in via Encoders::init_pio() — Phase 1c² will finalise it.)

#include "picoadk/hal/encoders.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include <cstring>

namespace picoadk::Encoders {

namespace {

constexpr std::size_t kMaxEncoders = 16;

struct State {
    EncoderPins pins;
    uint8_t     prev_ab;
    int32_t     position;
    int32_t     last_position;
    bool        btn_state;
    bool        btn_prev;
    uint32_t    btn_t_change_us;
};

State        g_state[kMaxEncoders];
std::size_t  g_count = 0;

// Two-bit gray-code transition table: index = (prev<<2) | curr,
// values: -1, 0, +1 indicating direction.
constexpr int8_t kTransition[16] = {
    0, +1, -1, 0,
   -1,  0,  0,+1,
   +1,  0,  0,-1,
    0, -1, +1, 0,
};

}  // namespace

bool init(const EncoderPins* pins, std::size_t count) {
    if (count > kMaxEncoders) return false;
    std::memset(g_state, 0, sizeof(g_state));
    g_count = count;
    for (std::size_t i = 0; i < count; ++i) {
        g_state[i].pins = pins[i];
        gpio_init(pins[i].pin_a); gpio_set_dir(pins[i].pin_a, GPIO_IN); gpio_pull_up(pins[i].pin_a);
        gpio_init(pins[i].pin_b); gpio_set_dir(pins[i].pin_b, GPIO_IN); gpio_pull_up(pins[i].pin_b);
        if (pins[i].pin_btn != 255) {
            gpio_init(pins[i].pin_btn);
            gpio_set_dir(pins[i].pin_btn, GPIO_IN);
            gpio_pull_up(pins[i].pin_btn);
        }
        uint8_t a = gpio_get(pins[i].pin_a) ? 1 : 0;
        uint8_t b = gpio_get(pins[i].pin_b) ? 1 : 0;
        g_state[i].prev_ab = (a << 1) | b;
    }
    return true;
}

void poll(EncoderReading* out, std::size_t count) {
    if (count > g_count) count = g_count;
    uint32_t now_us = to_us_since_boot(get_absolute_time());
    for (std::size_t i = 0; i < count; ++i) {
        State& s = g_state[i];
        uint8_t a = gpio_get(s.pins.pin_a) ? 1 : 0;
        uint8_t b = gpio_get(s.pins.pin_b) ? 1 : 0;
        uint8_t curr = (a << 1) | b;
        int8_t d = kTransition[(s.prev_ab << 2) | curr];
        if (s.pins.invert) d = -d;
        s.prev_ab = curr;
        if (s.pins.half_step || (curr == 0b00)) {
            s.position += d;
        }

        bool btn_now = false;
        bool btn_pressed = false, btn_released = false;
        if (s.pins.pin_btn != 255) {
            bool raw = !gpio_get(s.pins.pin_btn);
            if (raw != s.btn_state && (now_us - s.btn_t_change_us) > 4000) {
                s.btn_t_change_us = now_us;
                if (raw && !s.btn_state)  btn_pressed  = true;
                if (!raw && s.btn_state)  btn_released = true;
                s.btn_state = raw;
            }
            btn_now = s.btn_state;
        }
        out[i].position        = s.position;
        out[i].delta           = (int8_t)(s.position - s.last_position);
        out[i].button_down     = btn_now;
        out[i].button_pressed  = btn_pressed;
        out[i].button_released = btn_released;
        s.last_position = s.position;
    }
}

void reset(std::size_t i) {
    if (i < g_count) { g_state[i].position = 0; g_state[i].last_position = 0; }
}

}  // namespace picoadk::Encoders
