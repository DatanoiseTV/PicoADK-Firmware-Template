// Filter models with per-board optimised math.
//
// Each filter exposes the same shape — `reset(sr)`, parameter setters, and
// `process(x)` returning the filtered sample. The implementation in
// `src/dsp/filters.cpp` picks the fastest path the silicon offers:
//
//   * RP2040 (Cortex-M0+, no FPU)
//       — Q15/Q31 fixed-point math via dsp::math helpers.
//       — Hardware divider (`pico/divider.h`) for any infrequent divides.
//
//   * RP2350 (Cortex-M33, single-precision FPU + DSP extensions)
//       — Native float math; the M33 multiplies + fma in 1 cycle.
//       — Where two paired Q15 MACs would be a hot loop (biquad), the
//         DSP intrinsic `__smlad` is used (compiles to one instruction).
//
// User code never has to choose; `dsp::Real` is the right type for the
// current board, and the filters work the same on both.

#pragma once

#include "picoadk/dsp/real.h"

namespace picoadk::dsp {

// =============================================================================
// One-pole — cheapest LP/HP, single multiply per sample. Useful for envelope
// followers, signal smoothing, gentle de-essers.
//
// TPT (topology-preserving transform) form so cutoff sweeps stay stable
// even up to Nyquist/2.
// =============================================================================
class OnePole {
public:
    enum class Mode : uint8_t { LowPass, HighPass };

    void reset(float sr);
    void set_freq_hz(float hz);
    void set_mode(Mode m) { mode_ = m; }
    Real process(Real x);

private:
    float sr_   = 48000.0f;
    float g_    = 0.0f;     // tan(pi*fc/sr) / (1 + tan(...))
    float z1_   = 0.0f;
    Mode  mode_ = Mode::LowPass;
};

// =============================================================================
// Biquad — direct form II transposed. Coefficients can be set directly or
// computed for the standard RBJ cookbook responses (LP/HP/BP/notch/peak/
// low-shelf/high-shelf).
//
// Hot loop compiles to one MAC per stage on M33 via __smlad (paired Q15).
// On M0+ falls back to two int32 multiplies — still ~30 cycles per sample.
// =============================================================================
class Biquad {
public:
    void reset(float sr);
    void set_coefficients(float b0, float b1, float b2, float a1, float a2);

    void set_lowpass (float fc_hz, float q);
    void set_highpass(float fc_hz, float q);
    void set_bandpass(float fc_hz, float q);
    void set_notch   (float fc_hz, float q);
    void set_peak    (float fc_hz, float q, float gain_db);
    void set_lowshelf (float fc_hz, float q, float gain_db);
    void set_highshelf(float fc_hz, float q, float gain_db);

    Real process(Real x);

private:
    float sr_ = 48000.0f;
    float b0_ = 1.0f, b1_ = 0.0f, b2_ = 0.0f, a1_ = 0.0f, a2_ = 0.0f;
    float z1_ = 0.0f, z2_ = 0.0f;
};

// =============================================================================
// Moog ZDF ladder — virtual analogue, zero-delay-feedback. Self-oscillates
// at resonance ≈ 1.0; smooth response right up to resonance peak.
//
// Closer to the "real" Moog sound than the simpler Stilson-Smith model in
// `LadderFilter`. Costs ~4× more — fine on M33, marginal on M0+ (use the
// simpler model for >4-voice polysynths on RP2040).
// =============================================================================
class MoogLadderZdf {
public:
    void  reset(float sr);
    void  set_freq_hz(float hz);
    void  set_resonance(float r);   // 0..~1.05 (oscillation at ≈ 1.0)
    Real  process(Real x);

private:
    float sr_  = 48000.0f;
    float fc_  = 1000.0f;
    float res_ = 0.0f;
    float g_   = 0.0f;
    float G_   = 0.0f;
    float s_[4] = {0, 0, 0, 0};
};

// =============================================================================
// DC blocker — single-pole HP at ~5 Hz, removes any DC offset that's been
// introduced by clipping / saturation / asymmetric waveshaping.
//
//     y[n] = x[n] - x[n-1] + 0.995 * y[n-1]
// =============================================================================
class DcBlocker {
public:
    void  reset()                    { x1_ = y1_ = 0; }
    Real  process(Real x);

private:
    float x1_ = 0, y1_ = 0;
};

// =============================================================================
// Allpass — first-order, frequency-dependent phase rotation. Stack N of these
// (driven by the same coefficient) for a phaser; chain at audio rate for
// reverb diffusers.
// =============================================================================
class AllPass1 {
public:
    void  reset()                              { z1_ = 0; }
    void  set_coefficient(float a)             { a_ = a; }
    Real  process(Real x);

private:
    float a_  = 0.0f;
    float z1_ = 0.0f;
};

// =============================================================================
// Comb filter — bring-your-own buffer (SRAM or PSRAM via the FX delay
// helpers). Building block for Karplus-Strong, chorus, flanger, plate
// reverb tap structures.
// =============================================================================
class Comb {
public:
    void  configure(Real* buffer, std::size_t length);
    void  set_delay_samples(std::size_t s);
    void  set_feedback(float fb)               { fb_ = fb; }
    void  set_damping(float damp)              { damp_ = damp; }   // 0 = none
    Real  process(Real x);

private:
    Real*       buf_   = nullptr;
    std::size_t len_   = 0;
    std::size_t write_ = 0;
    std::size_t delay_ = 0;
    float       fb_    = 0.5f;
    float       damp_  = 0.0f;
    float       lp_z1_ = 0.0f;
};

}  // namespace picoadk::dsp
