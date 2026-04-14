// Display HAL — built on u8g2.
//
// u8g2 (lib/u8g2 submodule, BSD-2) provides the actual panel drivers, fonts,
// and drawing primitives. picoadk::Display owns the u8g2 instance and the
// byte/gpio callbacks that talk to the Pico SDK I2C/SPI peripherals (DMA on
// SPI for fast partial updates).
//
// User code can either use u8g2 directly:
//
//     auto& u = picoadk::Display::u8g2();
//     u8g2_ClearBuffer(&u);
//     u8g2_SetFont(&u, u8g2_font_6x10_tr);
//     u8g2_DrawStr(&u, 0, 10, "hello");
//     picoadk::Display::present();
//
// Or use the thin `gfx::Canvas`-style sugar for the common cases.

#pragma once

#include <cstdint>

extern "C" {
#include "u8g2.h"
}

namespace picoadk {

enum class DisplayDriver : uint8_t {
    SSD1306_128x64,
    SSD1306_128x32,
    SH1106_128x64,
    SSD1309_128x64,
    Custom,                 // pre-built u8g2 setup function passed in via setup_cb
};

enum class DisplayBus : uint8_t { I2C, Spi };

struct DisplayConfig {
    DisplayDriver driver       = DisplayDriver::SSD1306_128x64;
    DisplayBus    bus          = DisplayBus::I2C;

    // I2C
    uint8_t       i2c_address  = 0x3C;
    uint32_t      i2c_baud_hz  = 1'000'000;       // SSD1306 is happy at FM+

    // SPI (when bus == Spi)
    uint8_t       spi_dc_pin   = 255;
    uint8_t       spi_cs_pin   = 255;
    uint8_t       spi_rst_pin  = 255;
    uint32_t      spi_baud_hz  = 20'000'000;

    bool          flip_h       = false;
    bool          flip_v       = false;

    // For DisplayDriver::Custom — the u8g2 setup function (one of the
    // u8g2_Setup_* generated functions). Pass nullptr to use our defaults.
    void          (*custom_setup)(u8g2_t*, const u8g2_cb_t*, u8x8_msg_cb, u8x8_msg_cb) = nullptr;
};

namespace Display {

bool init(const DisplayConfig& cfg = {});
bool present_avail();   // true if the panel ACKed during init

// Direct access to the underlying u8g2 instance — full library is yours.
u8g2_t&  u8g2();

// Push the back buffer to the panel (DMA when on SPI).
void     present();

// Convenience: clear + present.
void     clear();

// Brightness 0..255 (where the panel supports it).
void     set_contrast(uint8_t c);

// True after `present()` finishes the DMA transfer; useful when scheduling
// frame updates not to overlap with audio-rate work.
bool     present_busy();

}  // namespace Display
}  // namespace picoadk
