// Numeric type abstraction for DSP code.
//
// On RP2040 (no FPU) the default Real type is a 16.16 fixed-point value;
// on RP2350 (single-precision FPU) it's `float`. User DSP code that uses
// `picoadk::dsp::Real` and the helper functions below builds unmodified for
// both targets.

#pragma once

#include "picoadk_board.h"

#include <cmath>
#include <cstdint>

namespace picoadk::dsp {

#if PICOADK_HAS_FPU

using Real = float;

inline constexpr Real from_float(float x) noexcept { return x; }
inline constexpr float to_float(Real x)   noexcept { return x; }

inline Real sin_(Real x)  noexcept { return std::sin(x); }
inline Real cos_(Real x)  noexcept { return std::cos(x); }
inline Real tan_(Real x)  noexcept { return std::tan(x); }
inline Real exp_(Real x)  noexcept { return std::exp(x); }
inline Real sqrt_(Real x) noexcept { return std::sqrt(x); }
inline Real tanh_(Real x) noexcept { return std::tanh(x); }

inline constexpr Real mul(Real a, Real b)         noexcept { return a * b; }
inline constexpr Real mac(Real acc, Real a, Real b) noexcept { return acc + a * b; }
inline constexpr Real div(Real a, Real b)         noexcept { return a / b; }
inline constexpr Real abs_(Real a)                noexcept { return a < 0 ? -a : a; }

inline constexpr Real clip(Real x, Real lo = Real(-1), Real hi = Real(1)) noexcept {
    return x < lo ? lo : (x > hi ? hi : x);
}

#else  // RP2040 — fixed-point Q16

using Real = int32_t;
inline constexpr int32_t kFracBits = 16;
inline constexpr int32_t kOne      = int32_t(1) << kFracBits;

inline constexpr Real from_float(float x) noexcept {
    return static_cast<Real>(x * float(kOne));
}
inline constexpr float to_float(Real x) noexcept {
    return float(x) / float(kOne);
}

inline constexpr Real mul(Real a, Real b) noexcept {
    return static_cast<Real>((static_cast<int64_t>(a) * b) >> kFracBits);
}
inline constexpr Real mac(Real acc, Real a, Real b) noexcept {
    return acc + mul(a, b);
}
Real div(Real a, Real b) noexcept;       // implemented in dsp/math.cpp (uses RP2040 hw divider)
Real sin_(Real x) noexcept;
Real cos_(Real x) noexcept;
Real tan_(Real x) noexcept;
Real exp_(Real x) noexcept;
Real sqrt_(Real x) noexcept;
Real tanh_(Real x) noexcept;

inline constexpr Real abs_(Real a) noexcept { return a < 0 ? -a : a; }
inline constexpr Real clip(Real x,
                           Real lo = -kOne,
                           Real hi =  kOne) noexcept {
    return x < lo ? lo : (x > hi ? hi : x);
}

#endif

// Convert Real -> int32 audio sample (signed, full-range).
inline int32_t to_int32_sample(Real x) noexcept {
#if PICOADK_HAS_FPU
    float c = clip(x, Real(-1), Real(1));
    return static_cast<int32_t>(c * 2147483520.0f);
#else
    Real c = clip(x);
    return c << 15;   // Q16 -> Q31
#endif
}

// Convert int32 audio sample -> Real.
inline Real from_int32_sample(int32_t s) noexcept {
#if PICOADK_HAS_FPU
    return float(s) * (1.0f / 2147483648.0f);
#else
    return s >> 15;   // Q31 -> Q16
#endif
}

}  // namespace picoadk::dsp
