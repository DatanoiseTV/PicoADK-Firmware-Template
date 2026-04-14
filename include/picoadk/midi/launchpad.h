// Novation Launchpad — programmer mode driver.
//
// Talks to a Launchpad Mini Mk3 / X / Pro Mk3 connected via the UsbHost
// transport. Entering programmer mode bypasses the in-device session logic so
// the firmware has direct control of every pad and side button.
//
//     picoadk::launchpad::Driver lp;
//     lp.on_pad_pressed = [](uint8_t x, uint8_t y, uint8_t velocity) { ... };
//     lp.set_pad_rgb(3, 4, 255, 0, 0);
//
// Coordinates are 0..7 for the main grid; the side and top button strips use
// negative or 8..N coordinates depending on model — see ButtonId helpers.

#pragma once

#include <cstdint>

namespace picoadk::launchpad {

enum class Model : uint8_t {
    Unknown = 0,
    MiniMk3,
    X,
    ProMk3,
};

enum class LedMode : uint8_t { Static, Flash, Pulse };

// Identifier for buttons that aren't on the 8x8 grid (side strip, top row).
enum class ButtonId : uint8_t {
    Up = 91, Down, Left, Right, Session, Drums, Keys, User,
    // ProMk3 extras …
};

struct Driver {
    using PadCallback    = void (*)(uint8_t x, uint8_t y, uint8_t velocity);
    using ButtonCallback = void (*)(ButtonId id, bool down);

    PadCallback    on_pad_pressed  = nullptr;
    PadCallback    on_pad_released = nullptr;
    ButtonCallback on_button       = nullptr;

    // Connect: enter programmer mode and identify the model. Returns false if
    // no Launchpad is currently enumerated.
    bool connect();
    void disconnect();

    Model model() const noexcept;
    bool  connected() const noexcept;

    // Per-pad LED control (programmer mode coordinate system).
    void set_pad_index   (uint8_t pad_index, uint8_t palette);
    void set_pad         (uint8_t x, uint8_t y, uint8_t palette);
    void set_pad_rgb     (uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
    void set_pad_mode    (uint8_t x, uint8_t y, LedMode mode);
    void set_button      (ButtonId id, uint8_t palette);

    // Bulk write: 64 palette indices for the grid + optional side strip data.
    void blit_grid_palette(const uint8_t (&grid)[8][8]);
    void blit_grid_rgb    (const uint8_t (&rgb)[8][8][3]);

    void clear();

    // Pump host-side MIDI; call from your MIDI task.
    void process();
};

}  // namespace picoadk::launchpad
