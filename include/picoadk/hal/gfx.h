// 1-bpp graphics primitives.
//
// Backend-agnostic: a `Canvas` owns a frame buffer and exposes draw calls;
// drivers (SSD1306, SH1106, SSD1309, ST7567, ...) push the buffer to the
// panel. Drawing code stays the same regardless of which OLED ships on the
// board.
//
// Fonts are statically linked PROGMEM-style tables; the default 6x8 font is
// always available, larger fonts can be added via `gfx::register_font`.

#pragma once

#include <cstddef>
#include <cstdint>

namespace picoadk::gfx {

struct Font {
    const uint8_t* glyphs;
    uint8_t        glyph_w;
    uint8_t        glyph_h;
    uint8_t        first_char;
    uint8_t        last_char;
    uint8_t        bytes_per_glyph;
};

extern const Font font6x8;
extern const Font font8x16;

class Canvas {
public:
    Canvas(uint8_t* buf, uint16_t w, uint16_t h)
        : buf_(buf), w_(w), h_(h), stride_(w / 8) {}

    uint16_t width()  const noexcept { return w_; }
    uint16_t height() const noexcept { return h_; }
    uint8_t* data()   const noexcept { return buf_; }

    void clear(bool white = false) noexcept;
    void pixel(int16_t x, int16_t y, bool on = true) noexcept;
    void hline(int16_t x, int16_t y, int16_t w, bool on = true) noexcept;
    void vline(int16_t x, int16_t y, int16_t h, bool on = true) noexcept;
    void line (int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool on = true) noexcept;
    void rect (int16_t x, int16_t y, int16_t w, int16_t h, bool on = true) noexcept;
    void fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, bool on = true) noexcept;
    void circle(int16_t cx, int16_t cy, int16_t r, bool on = true) noexcept;
    void fill_circle(int16_t cx, int16_t cy, int16_t r, bool on = true) noexcept;

    // Bit-blit a 1bpp bitmap stored row-major, MSB left.
    void blit(int16_t x, int16_t y, const uint8_t* src,
              uint16_t w, uint16_t h, bool on = true) noexcept;

    // Text. `cursor` advances. Returns end-x.
    int16_t text(int16_t x, int16_t y, const char* str,
                 const Font& font = font6x8, bool on = true) noexcept;
    int16_t printf(int16_t x, int16_t y, const Font& font, const char* fmt, ...) noexcept;

private:
    uint8_t* buf_;
    uint16_t w_;
    uint16_t h_;
    uint16_t stride_;
};

}  // namespace picoadk::gfx
