// Display HAL — drives 1-bpp panels (SSD1306, SH1106, SSD1309, ...).
//
// The HAL owns a `gfx::Canvas` whose frame buffer is pushed to the panel by
// `present()`. Application code never talks to the panel directly:
//
//     auto& g = picoadk::Display::canvas();
//     g.clear();
//     g.text(0, 0, "hello world");
//     picoadk::Display::present();

#pragma once

#include <cstdint>

#include "picoadk/hal/gfx.h"

namespace picoadk {

enum class DisplayDriver : uint8_t { SSD1306, SH1106, SSD1309 };

struct DisplayConfig {
    DisplayDriver driver  = DisplayDriver::SSD1306;
    uint8_t       i2c_address = 0x3C;
    uint16_t      width   = 128;
    uint16_t      height  = 64;
    bool          flip_h  = false;
    bool          flip_v  = false;
    uint32_t      i2c_baud = 1'000'000;   // SSD1306 happily takes 1 MHz Fast-Mode+
};

namespace Display {

bool init(const DisplayConfig& cfg = {});
bool present_avail();         // true if a panel ACKed at init time

gfx::Canvas& canvas();
void present();               // DMA frame buffer to panel
void clear();                 // shortcut for canvas().clear() + present()
void set_contrast(uint8_t c); // 0..255

}  // namespace Display
}  // namespace picoadk
