// DSP math helpers — Phase 0 stubs.
//
// Phase 2 fills in the RP2040 hardware-divider, M33 SIMD/DCP, and CMSIS-DSP
// integration. For now everything routes through portable C so user code that
// references picoadk::dsp::* still compiles.

#include "picoadk/dsp/math.h"
#include "picoadk/dsp/real.h"

#include <algorithm>
#include <cmath>

namespace picoadk::dsp {

#if !PICOADK_HAS_FPU
// Q16 fallbacks — Phase 2 will replace with hardware-divider versions on RP2040.

Real div(Real a, Real b) noexcept {
    if (b == 0) return 0;
    return static_cast<Real>((static_cast<int64_t>(a) << kFracBits) / b);
}
Real sin_(Real x)  noexcept { return from_float(std::sin (to_float(x))); }
Real cos_(Real x)  noexcept { return from_float(std::cos (to_float(x))); }
Real tan_(Real x)  noexcept { return from_float(std::tan (to_float(x))); }
Real exp_(Real x)  noexcept { return from_float(std::exp (to_float(x))); }
Real sqrt_(Real x) noexcept { return from_float(std::sqrt(to_float(x))); }
Real tanh_(Real x) noexcept { return from_float(std::tanh(to_float(x))); }
#endif

int32_t qadd_sat(int32_t a, int32_t b) noexcept {
    int64_t r = int64_t(a) + b;
    if (r >  INT32_MAX) return  INT32_MAX;
    if (r <  INT32_MIN) return  INT32_MIN;
    return int32_t(r);
}
int32_t qsub_sat(int32_t a, int32_t b) noexcept {
    int64_t r = int64_t(a) - b;
    if (r >  INT32_MAX) return  INT32_MAX;
    if (r <  INT32_MIN) return  INT32_MIN;
    return int32_t(r);
}
int32_t smlad_q15(uint32_t ab, uint32_t cd, int32_t acc) noexcept {
    int16_t a = int16_t(ab & 0xFFFF), b = int16_t(ab >> 16);
    int16_t c = int16_t(cd & 0xFFFF), d = int16_t(cd >> 16);
    return acc + int32_t(a) * c + int32_t(b) * d;
}

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
        out[i] = int32_t(c * 2147483520.0f);
    }
}
void int32_to_float(const int32_t* in, float* out, std::size_t n) noexcept {
    constexpr float k = 1.0f / 2147483648.0f;
    for (std::size_t i = 0; i < n; ++i) out[i] = float(in[i]) * k;
}

float lut_lerp_f(const float* t, std::size_t size, float phase) noexcept {
    float idx_f = phase * float(size);
    std::size_t i0 = std::size_t(idx_f) % size;
    std::size_t i1 = (i0 + 1) % size;
    float frac = idx_f - std::floor(idx_f);
    return t[i0] + (t[i1] - t[i0]) * frac;
}
Real lut_lerp(const Real* t, std::size_t size, float phase) noexcept {
#if PICOADK_HAS_FPU
    return lut_lerp_f(t, size, phase);
#else
    float idx_f = phase * float(size);
    std::size_t i0 = std::size_t(idx_f) % size;
    std::size_t i1 = (i0 + 1) % size;
    float frac = idx_f - std::floor(idx_f);
    return t[i0] + Real((int64_t(t[i1] - t[i0]) * int64_t(frac * float(kOne))) >> kFracBits);
#endif
}

}  // namespace picoadk::dsp
