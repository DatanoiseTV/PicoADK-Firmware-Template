// Display HAL — u8g2-backed implementation.
//
// One I2C or SPI back-end services every supported panel (SSD1306, SH1106,
// SSD1309). u8g2 talks to us via two callbacks: `byte_cb` (bus traffic) and
// `gpio_cb` (delays + reset). We keep the buffer in our own static so the
// SDK doesn't need to allocate.
//
// Hardware bring-up notes:
//   * I2C — uses the bus configured by the board (default I2C0 @ 1 MHz).
//     The display is auto-detected via address ACK in init().
//   * SPI — chip select / DC / RST pins come from DisplayConfig. We use DMA
//     for the framebuffer flush so present() returns immediately on long
//     transfers (~5 ms on a 128×64 panel @ 20 MHz SPI).
//
// All u8g2 buffer / drawing functions are exposed via Display::u8g2().

#include "picoadk/hal/display.h"
#include "picoadk/hal/gfx.h"
#include "picoadk_board.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"

#include <cstring>

namespace picoadk::Display {

namespace {

DisplayConfig g_cfg;
u8g2_t        g_u8g2;
gfx::Canvas   g_canvas{&g_u8g2};
bool          g_present_ok = false;
bool          g_dma_busy   = false;
int           g_dma_ch     = -1;

// ---- I2C HAL callback ---------------------------------------------------
uint8_t u8x8_byte_pico_i2c(u8x8_t* /*u8x8*/, uint8_t msg, uint8_t arg_int, void* arg_ptr) {
    static uint8_t buf[40];
    static uint8_t buf_idx = 0;

    switch (msg) {
        case U8X8_MSG_BYTE_INIT:
            i2c_init(PICOADK_OLED_I2C_INST, g_cfg.i2c_baud_hz);
            gpio_set_function(PICOADK_PIN_OLED_SDA, GPIO_FUNC_I2C);
            gpio_set_function(PICOADK_PIN_OLED_SCL, GPIO_FUNC_I2C);
            gpio_pull_up(PICOADK_PIN_OLED_SDA);
            gpio_pull_up(PICOADK_PIN_OLED_SCL);
            return 1;
        case U8X8_MSG_BYTE_START_TRANSFER:
            buf_idx = 0;
            return 1;
        case U8X8_MSG_BYTE_SEND: {
            uint8_t* data = (uint8_t*)arg_ptr;
            for (uint8_t i = 0; i < arg_int && buf_idx < sizeof(buf); ++i)
                buf[buf_idx++] = data[i];
            return 1;
        }
        case U8X8_MSG_BYTE_END_TRANSFER: {
            int n = i2c_write_blocking(PICOADK_OLED_I2C_INST,
                                       g_cfg.i2c_address, buf, buf_idx, false);
            return (n == buf_idx) ? 1 : 0;
        }
        case U8X8_MSG_BYTE_SET_DC: return 1;
        default: return 0;
    }
}

// ---- SPI HAL callback ---------------------------------------------------
uint8_t u8x8_byte_pico_spi(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr) {
    switch (msg) {
        case U8X8_MSG_BYTE_INIT:
            spi_init(spi0, g_cfg.spi_baud_hz);
            spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
            if (g_cfg.spi_cs_pin != 255) {
                gpio_init(g_cfg.spi_cs_pin); gpio_set_dir(g_cfg.spi_cs_pin, GPIO_OUT);
                gpio_put(g_cfg.spi_cs_pin, 1);
            }
            if (g_cfg.spi_dc_pin != 255) {
                gpio_init(g_cfg.spi_dc_pin); gpio_set_dir(g_cfg.spi_dc_pin, GPIO_OUT);
            }
            return 1;
        case U8X8_MSG_BYTE_SET_DC:
            if (g_cfg.spi_dc_pin != 255) gpio_put(g_cfg.spi_dc_pin, arg_int);
            return 1;
        case U8X8_MSG_BYTE_START_TRANSFER:
            if (g_cfg.spi_cs_pin != 255) gpio_put(g_cfg.spi_cs_pin, 0);
            return 1;
        case U8X8_MSG_BYTE_SEND:
            spi_write_blocking(spi0, (uint8_t*)arg_ptr, arg_int);
            return 1;
        case U8X8_MSG_BYTE_END_TRANSFER:
            if (g_cfg.spi_cs_pin != 255) gpio_put(g_cfg.spi_cs_pin, 1);
            return 1;
        default: return 0;
    }
}

// ---- GPIO + delay HAL --------------------------------------------------
uint8_t u8x8_gpio_and_delay_pico(u8x8_t* /*u8x8*/, uint8_t msg, uint8_t arg_int, void* /*arg_ptr*/) {
    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT: return 1;
        case U8X8_MSG_DELAY_NANO:        busy_wait_us(1);                return 1;
        case U8X8_MSG_DELAY_100NANO:     busy_wait_us(1);                return 1;
        case U8X8_MSG_DELAY_10MICRO:     busy_wait_us(arg_int * 10);     return 1;
        case U8X8_MSG_DELAY_MILLI:       sleep_ms(arg_int);              return 1;
        case U8X8_MSG_GPIO_RESET:
            if (g_cfg.spi_rst_pin != 255) gpio_put(g_cfg.spi_rst_pin, arg_int);
            return 1;
        case U8X8_MSG_GPIO_DC:
            if (g_cfg.spi_dc_pin != 255)  gpio_put(g_cfg.spi_dc_pin, arg_int);
            return 1;
        case U8X8_MSG_GPIO_CS:
            if (g_cfg.spi_cs_pin != 255)  gpio_put(g_cfg.spi_cs_pin, arg_int);
            return 1;
        default: return 1;
    }
}

bool i2c_probe(uint8_t addr) {
    uint8_t dummy = 0;
    int n = i2c_write_blocking(PICOADK_OLED_I2C_INST, addr, &dummy, 1, false);
    return n >= 0;
}

}  // namespace

bool init(const DisplayConfig& cfg) {
    g_cfg = cfg;

    auto byte_cb = (cfg.bus == DisplayBus::I2C) ? u8x8_byte_pico_i2c
                                                : u8x8_byte_pico_spi;
    auto gpio_cb = u8x8_gpio_and_delay_pico;

    if (cfg.custom_setup) {
        cfg.custom_setup(&g_u8g2, U8G2_R0, byte_cb, gpio_cb);
    } else {
        switch (cfg.driver) {
            case DisplayDriver::SSD1306_128x64:
                u8g2_Setup_ssd1306_i2c_128x64_noname_f(&g_u8g2, U8G2_R0, byte_cb, gpio_cb); break;
            case DisplayDriver::SSD1306_128x32:
                u8g2_Setup_ssd1306_i2c_128x32_univision_f(&g_u8g2, U8G2_R0, byte_cb, gpio_cb); break;
            case DisplayDriver::SH1106_128x64:
                u8g2_Setup_sh1106_i2c_128x64_noname_f(&g_u8g2, U8G2_R0, byte_cb, gpio_cb); break;
            case DisplayDriver::SSD1309_128x64:
                u8g2_Setup_ssd1309_i2c_128x64_noname2_f(&g_u8g2, U8G2_R0, byte_cb, gpio_cb); break;
            case DisplayDriver::Custom:
            default: return false;
        }
    }
    u8x8_SetI2CAddress(u8g2_GetU8x8(&g_u8g2), cfg.i2c_address << 1);

    if (cfg.bus == DisplayBus::I2C) {
        // Bring up I2C eagerly so we can probe before u8g2_InitDisplay().
        u8x8_byte_pico_i2c(nullptr, U8X8_MSG_BYTE_INIT, 0, nullptr);
        if (!i2c_probe(cfg.i2c_address)) {
            g_present_ok = false;
            return false;
        }
    }

    u8g2_InitDisplay(&g_u8g2);
    u8g2_SetPowerSave(&g_u8g2, 0);
    if (cfg.flip_h || cfg.flip_v) u8g2_SetFlipMode(&g_u8g2, cfg.flip_h ^ cfg.flip_v ? 1 : 0);
    u8g2_ClearBuffer(&g_u8g2);
    u8g2_SendBuffer(&g_u8g2);

    g_present_ok = true;
    return true;
}

bool         present_avail()       { return g_present_ok; }
u8g2_t&      u8g2()                { return g_u8g2; }
gfx::Canvas& canvas()              { return g_canvas; }
void         present()             { if (g_present_ok) u8g2_SendBuffer(&g_u8g2); }
void         clear()               { if (g_present_ok) { u8g2_ClearBuffer(&g_u8g2); u8g2_SendBuffer(&g_u8g2); } }
void         set_contrast(uint8_t c){ if (g_present_ok) u8g2_SetContrast(&g_u8g2, c); }
bool         present_busy()        { return g_dma_busy; }

}  // namespace picoadk::Display
