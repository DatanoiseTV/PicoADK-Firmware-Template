// DSP math — hardware-accelerated where the silicon helps.
//
// RP2040: hardware divider for fix_div on Q16; interp0 for LUT lookups.
// RP2350: M33 DSP intrinsics (QADD/QSUB/SMLAD), single-precision FPU,
//         double-precision DCP coprocessor (used opportunistically).

#include "picoadk/dsp/math.h"
#include "picoadk/dsp/real.h"
#include "picoadk_board.h"

#include <algorithm>
#include <cmath>

#if PICOADK_BOARD_V1
#  include "pico/divider.h"
#  include "hardware/interp.h"
#endif

#if PICOADK_HAS_FPU && defined(__ARM_FEATURE_DSP)
#  include "arm_acle.h"
#  define PICOADK_USE_M33_DSP 1
#else
#  define PICOADK_USE_M33_DSP 0
#endif

namespace picoadk::dsp {

#if !PICOADK_HAS_FPU

Real div(Real a, Real b) noexcept {
    if (b == 0) return 0;
    // RP2040 hardware divider: 64/64 signed.
    return (Real)div_s64s64(((int64_t)a) << kFracBits, (int64_t)b);
}

// Q16 trig / exp / sqrt — pulled from the original PicoADK vultin.cpp.
// Phase 7 will replace with branch-free table-driven versions.
Real sin_(Real x) noexcept {
    Real x1 = (x % 0x6487e);
    uint8_t sign = (x1 > 0x3243f);
    Real x2 = (x1 % 0x3243f);
    Real x3 = (x2 > 0x1921f) ? (0x3243f + (-x2)) : x2;
    Real xp2 = mul(x3, x3);
    Real acc = mul(x3, kOne + mul((Real)0xffffd556 + mul(0x222, xp2), xp2));
    return sign ? -acc : acc;
}
Real cos_(Real x)  noexcept { return sin_(x + (Real)(0x3243f >> 1)); }
Real tan_(Real x)  noexcept { return div(sin_(x), cos_(x)); }
Real exp_(Real x)  noexcept { return from_float(std::exp (to_float(x))); }
Real sqrt_(Real x) noexcept { return from_float(std::sqrt(to_float(x))); }
Real tanh_(Real x) noexcept {
    Real e_x  = exp_(x);
    Real m_ex = exp_(-x);
    return div(e_x - m_ex, e_x + m_ex);
}
#endif  // !PICOADK_HAS_FPU

// ---- Saturating ops -----------------------------------------------------
int32_t qadd_sat(int32_t a, int32_t b) noexcept {
#if PICOADK_USE_M33_DSP
    return __qadd(a, b);
#else
    int64_t r = (int64_t)a + b;
    if (r >  INT32_MAX) return INT32_MAX;
    if (r <  INT32_MIN) return INT32_MIN;
    return (int32_t)r;
#endif
}
int32_t qsub_sat(int32_t a, int32_t b) noexcept {
#if PICOADK_USE_M33_DSP
    return __qsub(a, b);
#else
    int64_t r = (int64_t)a - b;
    if (r >  INT32_MAX) return INT32_MAX;
    if (r <  INT32_MIN) return INT32_MIN;
    return (int32_t)r;
#endif
}
int32_t smlad_q15(uint32_t ab, uint32_t cd, int32_t acc) noexcept {
#if PICOADK_USE_M33_DSP
    return __smlad(ab, cd, acc);
#else
    int16_t a = (int16_t)(ab & 0xFFFF), b = (int16_t)(ab >> 16);
    int16_t c = (int16_t)(cd & 0xFFFF), d = (int16_t)(cd >> 16);
    return acc + (int32_t)a * c + (int32_t)b * d;
#endif
}

// ---- Block ops ----------------------------------------------------------
void scale_block(const Real* in, Real* out, std::size_t n, Real g) noexcept {
    for (std::size_t i = 0; i < n; ++i) out[i] = mul(in[i], g);
}
void mac_block(const Real* a, const Real* b, Real* out, std::size_t n) noexcept {
    for (std::size_t i = 0; i < n; ++i) out[i] = mac(out[i], a[i], b[i]);
}
void clip_block(const Real* in, Real* out, std::size_t n) noexcept {
    for (std::size_t i = 0; i < n; ++i) out[i] = clip(in[i]);
}

void float_to_int32(const float* in, int32_t* out, std::size_t n) noexcept {
    for (std::size_t i = 0; i < n; ++i) {
        float c = std::clamp(in[i], -1.0f, 1.0f);
        out[i] = (int32_t)(c * 2147483520.0f);
    }
}
void int32_to_float(const int32_t* in, float* out, std::size_t n) noexcept {
    constexpr float k = 1.0f / 2147483648.0f;
    for (std::size_t i = 0; i < n; ++i) out[i] = (float)in[i] * k;
}

// ---- LUT helpers --------------------------------------------------------
float lut_lerp_f(const float* t, std::size_t size, float phase) noexcept {
    float idx_f = phase * (float)size;
    std::size_t i0 = (std::size_t)idx_f % size;
    std::size_t i1 = (i0 + 1) % size;
    float frac = idx_f - std::floor(idx_f);
    return t[i0] + (t[i1] - t[i0]) * frac;
}
Real lut_lerp(const Real* t, std::size_t size, float phase) noexcept {
#if PICOADK_HAS_FPU
    return lut_lerp_f(t, size, phase);
#else
    float idx_f = phase * (float)size;
    std::size_t i0 = (std::size_t)idx_f % size;
    std::size_t i1 = (i0 + 1) % size;
    float frac = idx_f - std::floor(idx_f);
    return t[i0] + (Real)(((int64_t)(t[i1] - t[i0]) * (int64_t)(frac * (float)kOne)) >> kFracBits);
#endif
}

}  // namespace picoadk::dsp
