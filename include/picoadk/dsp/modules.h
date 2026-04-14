// Common DSP modules — public API surface.
//
// Phase 7 fills these in. They all build on picoadk::dsp::Real so they work
// on both RP2040 (Q16) and RP2350 (float). API is class-based (RAII) and
// designed to be cheap to instantiate from PSRAM when needed.

#pragma once

#include <cstddef>
#include <cstdint>

#include "picoadk/dsp/real.h"

namespace picoadk::dsp {

// ---- Oscillators --------------------------------------------------------

class BlepOscillator {
public:
    enum class Shape : uint8_t { Saw, Square, Triangle };
    void  reset(float sample_rate);
    void  set_shape(Shape s);
    void  set_freq_hz(float hz);
    Real  process();
private:
    float sr_ = 48000.0f, phase_ = 0.0f, inc_ = 0.0f;
    Shape shape_ = Shape::Saw;
};

class WavetableOsc {
public:
    void  reset(float sample_rate);
    void  set_table(const Real* table, std::size_t size);
    void  set_freq_hz(float hz);
    Real  process();
private:
    const Real* tbl_ = nullptr;
    std::size_t size_ = 0;
    float sr_ = 48000.0f, phase_ = 0.0f, inc_ = 0.0f;
};

class Noise {
public:
    enum class Color : uint8_t { White, Pink, Brown };
    void  set_color(Color c);
    Real  process();
private:
    Color  color_ = Color::White;
    uint32_t state_ = 0xC001CAFEu;
    float  pink_b0_ = 0, pink_b1_ = 0, brown_ = 0;
};

// ---- Envelopes ----------------------------------------------------------

class Adsr {
public:
    void  reset(float sample_rate);
    void  set(float a, float d, float s, float r);
    void  gate(bool on);
    Real  process();
    bool  active() const noexcept { return state_ != 0; }
private:
    float sr_ = 48000.0f, a_ = 0.01f, d_ = 0.1f, s_ = 0.7f, r_ = 0.2f;
    float out_ = 0.0f;
    uint8_t state_ = 0;
};

// ---- Filters ------------------------------------------------------------

class StateVariableFilter {
public:
    enum class Mode : uint8_t { LP, BP, HP, Notch };
    void  reset(float sample_rate);
    void  set_freq_hz(float hz);
    void  set_q(float q);
    void  set_mode(Mode m);
    Real  process(Real x);
private:
    float sr_ = 48000.0f, f_ = 1000.0f, q_ = 0.7f;
    float ic1_ = 0, ic2_ = 0, g_ = 0, k_ = 0, a1_ = 0, a2_ = 0, a3_ = 0;
    Mode  mode_ = Mode::LP;
};

class LadderFilter {
public:
    void  reset(float sample_rate);
    void  set_freq_hz(float hz);
    void  set_resonance(float r);
    Real  process(Real x);
private:
    float sr_ = 48000.0f, f_ = 1000.0f, res_ = 0.2f;
    float z_[4]{};
};

// ---- Saturation / shaping ----------------------------------------------

class Saturator {
public:
    void  set_drive(float drive);   // 0..1
    Real  process(Real x);
private:
    float drive_ = 0.5f;
};

class Wavefolder {
public:
    void  set_amount(float a);      // 0..1
    Real  process(Real x);
private:
    float amount_ = 0.0f;
};

// ---- Time-based effects -------------------------------------------------

// Delay backed by user-supplied buffer (SRAM or PSRAM).
class DelayLine {
public:
    void  configure(Real* buffer, std::size_t length);
    void  set_time_samples(std::size_t s);
    void  set_feedback(float fb);
    void  set_mix(float dry_wet);
    Real  process(Real x);
private:
    Real* buf_ = nullptr;
    std::size_t len_ = 0, write_ = 0, time_ = 0;
    float fb_ = 0.5f, mix_ = 0.5f;
};

// FDN reverb. State buffers are user-supplied so RP2040 can keep them in
// SRAM and v2 can route them through PSRAM.
class FdnReverb {
public:
    void  configure(Real* state_buffer, std::size_t bytes);
    void  reset(float sample_rate);
    void  set_size(float size);     // 0..1
    void  set_damping(float damp);  // 0..1
    void  process(const Real* in_l, const Real* in_r,
                  Real* out_l, Real* out_r, std::size_t n);
};

// ---- Dynamics -----------------------------------------------------------

class Compressor {
public:
    void  reset(float sample_rate);
    void  set(float threshold_db, float ratio, float attack_ms, float release_ms);
    Real  process(Real x);
private:
    float sr_ = 48000.0f, thr_ = 0.5f, ratio_ = 4.0f;
    float atk_ = 0.0f, rel_ = 0.0f, env_ = 0.0f;
};

}  // namespace picoadk::dsp
