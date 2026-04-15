// Button HAL — debounced, with hold and double-click detection.
//
// Lock-free: state is updated from a periodic timer (default 1 kHz via the
// FreeRTOS timer service); user code reads the latest snapshot whenever it
// likes. Up to 32 buttons in one bank, several banks supported.

#pragma once

#include <cstddef>
#include <cstdint>

namespace picoadk {

struct ButtonPins {
    uint8_t pin;
    bool    active_low      = true;     // buttons-to-ground with pull-up
    bool    enable_pull     = true;     // configure internal pull on init
    uint16_t debounce_ms    = 8;
    uint16_t hold_ms        = 600;      // fires "held" event
    uint16_t double_click_ms = 250;
};

enum class ButtonEvent : uint8_t {
    None = 0,
    Pressed,        // edge: clean press after debounce
    Released,       // edge: clean release after debounce
    Held,           // crossed hold_ms while still pressed
    DoubleClicked,
};

struct ButtonReading {
    bool        down;
    ButtonEvent event;          // most recent edge since last poll (cleared on read)
    uint16_t    held_ms;        // 0 if not currently down
};

namespace Buttons {

bool init(const ButtonPins* pins, std::size_t count);
void poll(ButtonReading* out, std::size_t count);

}  // namespace Buttons
}  // namespace picoadk
