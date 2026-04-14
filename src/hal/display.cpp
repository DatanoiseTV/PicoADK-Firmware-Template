// Display HAL stub — Phase 1b wires u8g2 in for real (panel drivers, DMA SPI,
// I2C HAL callbacks). Phase 0 only exposes the API surface; the u8g2 instance
// lives here so user code can already compile against it.

#include "picoadk/hal/display.h"
#include "picoadk/hal/gfx.h"

namespace picoadk::Display {

namespace {
u8g2_t      g_u8g2;
bool        g_present = false;
gfx::Canvas g_canvas{&g_u8g2};
}

bool        init(const DisplayConfig&) { return false; }
bool        present_avail()            { return g_present; }
u8g2_t&     u8g2()                     { return g_u8g2; }
gfx::Canvas& canvas()                  { return g_canvas; }
void        present()                  {}
void        clear()                    { u8g2_ClearBuffer(&g_u8g2); present(); }
void        set_contrast(uint8_t c)    { u8g2_SetContrast(&g_u8g2, c); }
bool        present_busy()             { return false; }

}  // namespace picoadk::Display
