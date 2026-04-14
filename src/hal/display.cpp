// Display HAL stub — Phase 1b implements the SSD1306/SH1106/SSD1309 backend
// and the gfx::Canvas frame buffer. For Phase 0 we expose the API surface
// only.

#include "picoadk/hal/display.h"

namespace picoadk::Display {

namespace {
uint8_t       g_buf[128 * 64 / 8];
gfx::Canvas   g_canvas{g_buf, 128, 64};
bool          g_present = false;
}

bool init(const DisplayConfig&) { return false; }
bool present_avail()            { return g_present; }
gfx::Canvas& canvas()           { return g_canvas; }
void present()                  {}
void clear()                    { g_canvas.clear(); present(); }
void set_contrast(uint8_t)      {}

}  // namespace picoadk::Display
