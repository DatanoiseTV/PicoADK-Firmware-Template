// Common DSP modules — implementations.
//
// Float-first; on RP2040 the picoadk::dsp::Real conversions take care of the
// Q16↔float trips at the API boundary. None of these are clever — they're
// reference-quality starting points that user code can replace.

#include "picoadk/dsp/modules.h"
#include "picoadk/dsp/real.h"

#include <cmath>
#include <cstdlib>

namespace picoadk::dsp {

// ---- BlepOscillator -----------------------------------------------------
namespace {
inline float poly_blep(float t, float dt) {
    if (t < dt)        { float x = t / dt;          return x + x - x*x - 1.0f; }
    else if (t > 1.0f - dt) { float x = (t - 1.0f)/dt; return x*x + x + x + 1.0f; }
    return 0.0f;
}
}
void BlepOscillator::reset(float sr) { sr_ = sr; phase_ = 0.0f; }
void BlepOscillator::set_shape(Shape s) { shape_ = s; }
void BlepOscillator::set_freq_hz(float hz) { inc_ = hz / sr_; }
Real BlepOscillator::process() {
    float p = phase_;
    float v = 0.0f;
    switch (shape_) {
        case Shape::Saw:    v = 2.0f * p - 1.0f - poly_blep(p, inc_); break;
        case Shape::Square:
            v = (p < 0.5f) ? 1.0f : -1.0f;
            v += poly_blep(p, inc_);
            v -= poly_blep(std::fmod(p + 0.5f, 1.0f), inc_);
            break;
        case Shape::Triangle: {
            // integrate square — small enough that the residual DC is fine here
            float sq = (p < 0.5f) ? 1.0f : -1.0f;
            sq += poly_blep(p, inc_);
            sq -= poly_blep(std::fmod(p + 0.5f, 1.0f), inc_);
            static thread_local float tri = 0.0f;
            tri = inc_ * sq + (1.0f - inc_) * tri;
            v = 4.0f * tri;
        } break;
    }
    phase_ += inc_;
    if (phase_ >= 1.0f) phase_ -= 1.0f;
    return from_float(v);
}

// ---- WavetableOsc -------------------------------------------------------
void WavetableOsc::reset(float sr)               { sr_ = sr; phase_ = 0.0f; }
void WavetableOsc::set_table(const Real* t, std::size_t s) { tbl_ = t; size_ = s; }
void WavetableOsc::set_freq_hz(float hz)         { inc_ = hz / sr_; }
Real WavetableOsc::process() {
    if (!tbl_ || !size_) return Real(0);
    Real v = lut_lerp(tbl_, size_, phase_);
    phase_ += inc_;
    if (phase_ >= 1.0f) phase_ -= 1.0f;
    return v;
}

// ---- Noise --------------------------------------------------------------
void Noise::set_color(Color c) { color_ = c; }
Real Noise::process() {
    state_ = state_ * 1664525u + 1013904223u;
    float w = (float)((int32_t)state_) * (1.0f / 2147483648.0f);
    float out = 0.0f;
    switch (color_) {
        case Color::White: out = w; break;
        case Color::Pink:  pink_b1_ = 0.99f * pink_b1_ + 0.01f * w; out = pink_b1_; break;
        case Color::Brown: brown_   = 0.998f * brown_  + 0.02f * w; out = brown_;   break;
    }
    return from_float(out);
}

// ---- Adsr ---------------------------------------------------------------
void Adsr::reset(float sr)                                { sr_ = sr; out_ = 0.0f; state_ = 0; }
void Adsr::set(float a, float d, float s, float r)        { a_=a; d_=d; s_=s; r_=r; }
void Adsr::gate(bool on)                                  { state_ = on ? 1 : 4; }
Real Adsr::process() {
    auto step = [&](float seconds) { return 1.0f / (seconds * sr_ + 1.0f); };
    switch (state_) {
        case 1: out_ += (1.0f - out_) * step(a_); if (out_ >= 0.999f) state_ = 2; break;
        case 2: out_ += (s_ - out_) * step(d_);   if (std::fabs(out_-s_) < 0.001f) state_ = 3; break;
        case 3: /* sustain */ break;
        case 4: out_ += (0.0f - out_) * step(r_); if (out_ <= 0.001f) { out_ = 0; state_ = 0; } break;
        default: out_ = 0.0f; break;
    }
    return from_float(out_);
}

// ---- StateVariableFilter (Cytomic, Andy Simper's design) ---------------
void StateVariableFilter::reset(float sr) { sr_ = sr; ic1_ = ic2_ = 0; set_freq_hz(f_); set_q(q_); }
void StateVariableFilter::set_q(float q)  { q_ = q; k_ = 1.0f / q_; a1_ = 1.0f / (1.0f + g_*(g_+k_)); a2_ = g_*a1_; a3_ = g_*a2_; }
void StateVariableFilter::set_freq_hz(float hz) {
    f_ = hz;
    g_ = std::tan(3.14159265f * hz / sr_);
    a1_ = 1.0f / (1.0f + g_*(g_+k_)); a2_ = g_*a1_; a3_ = g_*a2_;
}
void StateVariableFilter::set_mode(Mode m) { mode_ = m; }
Real StateVariableFilter::process(Real x_in) {
    float x = to_float(x_in);
    float v3 = x - ic2_;
    float v1 = a1_*ic1_ + a2_*v3;
    float v2 = ic2_ + a2_*ic1_ + a3_*v3;
    ic1_ = 2.0f*v1 - ic1_;
    ic2_ = 2.0f*v2 - ic2_;
    float out = (mode_ == Mode::LP) ? v2
              : (mode_ == Mode::BP) ? v1
              : (mode_ == Mode::HP) ? (x - k_*v1 - v2)
              :                       (x - k_*v1);     // Notch
    return from_float(out);
}

// ---- LadderFilter (Stilson/Smith approximation) ------------------------
void LadderFilter::reset(float sr) { sr_ = sr; for (auto& z : z_) z = 0; }
void LadderFilter::set_freq_hz(float hz) { f_ = hz; }
void LadderFilter::set_resonance(float r) { res_ = r; }
Real LadderFilter::process(Real x_in) {
    float x = to_float(x_in);
    float fc = std::min(0.49f, f_ / sr_);
    float k  = 4.0f * res_;
    float input = x - k * z_[3];
    z_[0] += fc * (std::tanh(input)  - std::tanh(z_[0]));
    z_[1] += fc * (std::tanh(z_[0]) - std::tanh(z_[1]));
    z_[2] += fc * (std::tanh(z_[1]) - std::tanh(z_[2]));
    z_[3] += fc * (std::tanh(z_[2]) - std::tanh(z_[3]));
    return from_float(z_[3]);
}

// ---- Saturator ----------------------------------------------------------
void Saturator::set_drive(float d) { drive_ = d; }
Real Saturator::process(Real x_in) {
    float x = to_float(x_in) * (1.0f + 4.0f * drive_);
    return from_float(std::tanh(x));
}

// ---- Wavefolder ---------------------------------------------------------
void Wavefolder::set_amount(float a) { amount_ = a; }
Real Wavefolder::process(Real x_in) {
    float x = to_float(x_in) * (1.0f + 5.0f * amount_);
    while (x >  1.0f) x = 2.0f - x;
    while (x < -1.0f) x = -2.0f - x;
    return from_float(x);
}

// ---- DelayLine ----------------------------------------------------------
void DelayLine::configure(Real* buf, std::size_t len) { buf_ = buf; len_ = len; write_ = 0; for (std::size_t i = 0; i < len; ++i) buf[i] = 0; }
void DelayLine::set_time_samples(std::size_t s) { time_ = (s < len_) ? s : len_ - 1; }
void DelayLine::set_feedback(float fb) { fb_ = fb; }
void DelayLine::set_mix(float m)       { mix_ = m; }
Real DelayLine::process(Real x_in) {
    if (!buf_) return x_in;
    std::size_t r = (write_ + len_ - time_) % len_;
    Real wet = buf_[r];
    float fx = to_float(x_in) + to_float(wet) * fb_;
    buf_[write_] = from_float(fx);
    write_ = (write_ + 1) % len_;
    return from_float(to_float(x_in) * (1.0f - mix_) + to_float(wet) * mix_);
}

// ---- FdnReverb (skeletal) -----------------------------------------------
void FdnReverb::configure(Real*, std::size_t) {}
void FdnReverb::reset(float)    {}
void FdnReverb::set_size(float) {}
void FdnReverb::set_damping(float) {}
void FdnReverb::process(const Real* in_l, const Real* in_r,
                        Real* out_l, Real* out_r, std::size_t n) {
    // TODO Phase 7: 8x8 Hadamard FDN with absorbing comb filters.
    for (std::size_t i = 0; i < n; ++i) { out_l[i] = in_l[i]; out_r[i] = in_r[i]; }
}

// ---- Compressor ---------------------------------------------------------
void Compressor::reset(float sr)                                         { sr_ = sr; env_ = 0; }
void Compressor::set(float thr_db, float ratio, float atk_ms, float rel_ms) {
    thr_   = std::pow(10.0f, thr_db / 20.0f);
    ratio_ = ratio;
    atk_   = std::exp(-1.0f / (atk_ms * 0.001f * sr_));
    rel_   = std::exp(-1.0f / (rel_ms * 0.001f * sr_));
}
Real Compressor::process(Real x_in) {
    float x = to_float(x_in);
    float ax = std::fabs(x);
    env_ = (ax > env_) ? (atk_ * env_ + (1 - atk_) * ax)
                       : (rel_ * env_ + (1 - rel_) * ax);
    float gain = 1.0f;
    if (env_ > thr_) gain = thr_ + (env_ - thr_) / ratio_;
    if (env_ > 0)    gain /= env_;
    return from_float(x * gain);
}

}  // namespace picoadk::dsp
