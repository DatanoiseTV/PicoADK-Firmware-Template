// Hardware-accelerated DSP math helpers.
//
// These are thin wrappers over RP2040 / RP2350 features that DSP code wants
// constantly: fast multiply-accumulate, single-cycle clamps, lookup-table
// interpolation, fast trig.
//
// Where the silicon doesn't help (RP2040 has no SIMD; RP2350's M33 has DSP
// extensions but no NEON) the wrappers degrade gracefully to a plain C
// implementation so user code keeps building.

#pragma once

#include <cstddef>
#include <cstdint>

#include "picoadk/dsp/real.h"

namespace picoadk::dsp {

// ---- Fast scalar ops ----------------------------------------------------

// Saturating signed-add of two Q31 samples. Uses M33 QADD on RP2350.
int32_t qadd_sat(int32_t a, int32_t b) noexcept;

// Saturating signed-sub.
int32_t qsub_sat(int32_t a, int32_t b) noexcept;

// Q15 dual MAC: returns a0*b0 + a1*b1, useful for biquad inner loop.
// On M33 this maps to SMLAD.
int32_t smlad_q15(uint32_t ab_packed, uint32_t cd_packed, int32_t acc) noexcept;

// ---- Block ops ----------------------------------------------------------

// out[n] = in[n] * gain  for n in [0, count).
void scale_block(const Real* in, Real* out, std::size_t count, Real gain) noexcept;

// out[n] += a[n] * b[n]
void mac_block(const Real* a, const Real* b, Real* out, std::size_t count) noexcept;

// out[n] = clip(in[n], -1, +1)
void clip_block(const Real* in, Real* out, std::size_t count) noexcept;

// Convert a planar float channel to int32 for the I2S engine.
void float_to_int32(const float* in, int32_t* out, std::size_t count) noexcept;
void int32_to_float(const int32_t* in, float* out, std::size_t count) noexcept;

// ---- LUT helpers --------------------------------------------------------

// Linear-interpolated lookup. `phase` is in [0, 1). On RP2040 the hardware
// interpolator (interp0) is used; on RP2350 we still use the interp blocks.
float  lut_lerp_f(const float* table, std::size_t size, float phase) noexcept;
Real   lut_lerp  (const Real* table, std::size_t size, float phase) noexcept;

}  // namespace picoadk::dsp
