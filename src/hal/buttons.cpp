// Buttons HAL — debounced polling, with hold + double-click.
//
// One global FreeRTOS timer ticks at 1 kHz; sampling N buttons costs ~N µs
// regardless of pin count. State is stored in a small per-button struct and
// reads are lock-free (single-writer, single-reader).

#include "picoadk/hal/buttons.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include <FreeRTOS.h>
#include <timers.h>

#include <cstring>

namespace picoadk::Buttons {

namespace {

constexpr std::size_t kMax = 32;

struct State {
    ButtonPins  cfg;
    bool        raw;
    bool        debounced;
    bool        last_emitted;
    uint16_t    same_for_ms;
    uint16_t    held_ms;
    uint16_t    last_release_ms;
    uint32_t    age_ms;
    ButtonEvent pending;
};

State        g_state[kMax];
std::size_t  g_count = 0;
TimerHandle_t g_timer = nullptr;

void tick_cb(TimerHandle_t) {
    for (std::size_t i = 0; i < g_count; ++i) {
        State& s = g_state[i];
        bool now = s.cfg.active_low ? !gpio_get(s.cfg.pin) : gpio_get(s.cfg.pin);
        if (now == s.raw) {
            if (s.same_for_ms < 0xFFFF) s.same_for_ms++;
        } else {
            s.raw = now;
            s.same_for_ms = 0;
        }
        s.age_ms++;
        if (s.same_for_ms >= s.cfg.debounce_ms) {
            if (s.raw != s.debounced) {
                s.debounced = s.raw;
                if (s.debounced) {
                    s.held_ms = 0;
                    if ((uint16_t)(s.age_ms - s.last_release_ms) <= s.cfg.double_click_ms)
                        s.pending = ButtonEvent::DoubleClicked;
                    else
                        s.pending = ButtonEvent::Pressed;
                } else {
                    s.last_release_ms = (uint16_t)s.age_ms;
                    s.pending = ButtonEvent::Released;
                }
            }
        }
        if (s.debounced) {
            s.held_ms++;
            if (s.held_ms == s.cfg.hold_ms) s.pending = ButtonEvent::Held;
        }
    }
}

}  // namespace

bool init(const ButtonPins* pins, std::size_t count) {
    if (count > kMax) return false;
    std::memset(g_state, 0, sizeof(g_state));
    g_count = count;
    for (std::size_t i = 0; i < count; ++i) {
        g_state[i].cfg = pins[i];
        gpio_init(pins[i].pin);
        gpio_set_dir(pins[i].pin, GPIO_IN);
        if (pins[i].enable_pull) {
            if (pins[i].active_low) gpio_pull_up(pins[i].pin);
            else                    gpio_pull_down(pins[i].pin);
        }
    }
    if (!g_timer) {
        g_timer = xTimerCreate("BTN", pdMS_TO_TICKS(1), pdTRUE, nullptr, tick_cb);
        if (g_timer) xTimerStart(g_timer, 0);
    }
    return true;
}

void poll(ButtonReading* out, std::size_t count) {
    if (count > g_count) count = g_count;
    for (std::size_t i = 0; i < count; ++i) {
        out[i].down    = g_state[i].debounced;
        out[i].held_ms = g_state[i].debounced ? g_state[i].held_ms : 0;
        out[i].event   = g_state[i].pending;
        g_state[i].pending = ButtonEvent::None;
    }
}

}  // namespace picoadk::Buttons
