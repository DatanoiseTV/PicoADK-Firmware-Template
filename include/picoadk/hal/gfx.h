// Lightweight wrapper around u8g2's drawing primitives.
//
// u8g2 lives in lib/u8g2 (BSD-2) and is the source of truth for fonts and
// panel drivers — see <picoadk/hal/display.h> for direct access. The Canvas
// class here is a tiny C++ adapter so terse code reads naturally:
//
//     auto& g = picoadk::Display::canvas();
//     g.clear();
//     g.text(0, 10, "hello");
//     g.rect(0, 0, 64, 16);
//     picoadk::Display::present();
//
// Anything more involved — XBM blits, large fonts, scrolling, complex glyphs
// — should call u8g2 directly via picoadk::Display::u8g2().

#pragma once

#include <cstdarg>
#include <cstdint>

extern "C" {
#include "u8g2.h"
}

namespace picoadk::gfx {

class Canvas {
public:
    explicit Canvas(u8g2_t* u) : u_(u) {}

    u8g2_t* native() const noexcept { return u_; }

    uint16_t width()  const noexcept { return u8g2_GetDisplayWidth (u_); }
    uint16_t height() const noexcept { return u8g2_GetDisplayHeight(u_); }

    void clear()                                                                    noexcept { u8g2_ClearBuffer(u_); }
    void pixel(int16_t x, int16_t y, bool on = true)                                noexcept { u8g2_SetDrawColor(u_, on); u8g2_DrawPixel(u_, x, y); }
    void hline(int16_t x, int16_t y, int16_t w, bool on = true)                     noexcept { u8g2_SetDrawColor(u_, on); u8g2_DrawHLine(u_, x, y, w); }
    void vline(int16_t x, int16_t y, int16_t h, bool on = true)                     noexcept { u8g2_SetDrawColor(u_, on); u8g2_DrawVLine(u_, x, y, h); }
    void line (int16_t x0,int16_t y0,int16_t x1,int16_t y1, bool on = true)         noexcept { u8g2_SetDrawColor(u_, on); u8g2_DrawLine (u_, x0, y0, x1, y1); }
    void rect (int16_t x, int16_t y, int16_t w, int16_t h, bool on = true)          noexcept { u8g2_SetDrawColor(u_, on); u8g2_DrawFrame(u_, x, y, w, h); }
    void fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, bool on = true)      noexcept { u8g2_SetDrawColor(u_, on); u8g2_DrawBox  (u_, x, y, w, h); }
    void circle(int16_t cx, int16_t cy, int16_t r, bool on = true)                  noexcept { u8g2_SetDrawColor(u_, on); u8g2_DrawCircle(u_, cx, cy, r, U8G2_DRAW_ALL); }
    void fill_circle(int16_t cx, int16_t cy, int16_t r, bool on = true)             noexcept { u8g2_SetDrawColor(u_, on); u8g2_DrawDisc  (u_, cx, cy, r, U8G2_DRAW_ALL); }

    // Bit-blit an XBM bitmap (LSB-first per row, the format u8g2 expects).
    void blit(int16_t x, int16_t y, const uint8_t* xbm,
              uint16_t w, uint16_t h, bool on = true) noexcept {
        u8g2_SetDrawColor(u_, on);
        u8g2_DrawXBM(u_, x, y, w, h, xbm);
    }

    // Text. Default font is small (6x10). Returns end-x.
    int16_t text(int16_t x, int16_t y, const char* str,
                 const uint8_t* font = u8g2_font_6x10_tr,
                 bool on = true) noexcept {
        u8g2_SetFont(u_, font);
        u8g2_SetDrawColor(u_, on);
        u8g2_DrawStr(u_, x, y, str);
        return x + u8g2_GetStrWidth(u_, str);
    }

    int16_t printf_(int16_t x, int16_t y, const uint8_t* font, const char* fmt, ...) noexcept {
        char buf[64];
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(buf, sizeof(buf), fmt, ap);
        va_end(ap);
        return text(x, y, buf, font);
    }

private:
    u8g2_t* u_;
};

}  // namespace picoadk::gfx

namespace picoadk::Display { gfx::Canvas& canvas(); }
