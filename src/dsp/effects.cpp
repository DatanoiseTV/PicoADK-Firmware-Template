// Analog-flavoured effects.

#include "picoadk/dsp/effects.h"
#include "picoadk/dsp/real.h"
#include "picoadk/hal/psram.h"

#include <cmath>
#include <cstdlib>

namespace picoadk::dsp {

// ---- RingModulator -------------------------------------------------------
void RingModulator::reset(float sr) { sr_ = sr; phase_ = 0; set_carrier_hz(440.0f); }
void RingModulator::set_carrier_hz(float hz) { inc_ = hz / sr_; }
void RingModulator::set_mix(float m)         { mix_ = m < 0 ? 0 : (m > 1 ? 1 : m); }
Real RingModulator::process(Real x_in) {
    float x = to_float(x_in);
    float c = std::sin(2.0f * 3.14159265f * phase_);
    phase_ += inc_;
    if (phase_ >= 1.0f) phase_ -= 1.0f;
    else if (phase_ < 0.0f) phase_ += 1.0f;
    float wet = x * c;
    return from_float(x * (1.0f - mix_) + wet * mix_);
}

// ---- Phaser --------------------------------------------------------------
void Phaser::reset(float sr) {
    sr_ = sr;
    for (auto& a : ap_) a.reset();
    fb_state_ = 0.0f;
    lfo_phase_ = 0.0f;
    set_rate_hz(0.5f);
}
void Phaser::set_stages(int n) {
    if (n < 2)            n = 2;
    if (n > kMaxStages)   n = kMaxStages;
    stages_ = n;
}
void Phaser::set_rate_hz(float hz) { lfo_inc_ = hz / sr_; }
void Phaser::set_depth(float d)    { depth_  = d < 0 ? 0 : (d > 1 ? 1 : d); }
void Phaser::set_feedback(float f) { fb_     = f < 0 ? 0 : (f > 0.95f ? 0.95f : f); }
void Phaser::set_mix(float m)      { mix_    = m < 0 ? 0 : (m > 1 ? 1 : m); }

Real Phaser::process(Real x_in) {
    // LFO drives the all-pass coefficient. A coefficient `a` in [-1, +1]
    // shifts the all-pass break frequency exponentially; we map the LFO
    // into a musical sweep range.
    float lfo = std::sin(2.0f * 3.14159265f * lfo_phase_);
    lfo_phase_ += lfo_inc_;
    if (lfo_phase_ >= 1.0f) lfo_phase_ -= 1.0f;

    float a = -0.6f + 0.6f * depth_ * lfo;        // ~ -1.2 .. +0.0 sweep
    if (a < -0.99f) a = -0.99f; if (a > 0.99f) a = 0.99f;
    for (int i = 0; i < stages_; ++i) ap_[i].set_coefficient(a);

    float x  = to_float(x_in);
    float in = x + fb_state_ * fb_;
    float y  = in;
    for (int i = 0; i < stages_; ++i) y = to_float(ap_[i].process(from_float(y)));
    fb_state_ = y;
    return from_float(x * (1.0f - mix_) + y * mix_);
}

// ---- StereoEnsemble ------------------------------------------------------
bool StereoEnsemble::configure(float sample_rate, float max_delay_ms) {
    sr_  = sample_rate;
    len_ = (std::size_t)(max_delay_ms * 0.001f * sr_ + 0.5f);
    if (len_ < 64) len_ = 64;
    bool psram = Psram::available();
    auto* a = (Real*)(psram ? Psram::alloc(len_ * sizeof(Real), 8) : std::calloc(len_, sizeof(Real)));
    auto* b = (Real*)(psram ? Psram::alloc(len_ * sizeof(Real), 8) : std::calloc(len_, sizeof(Real)));
    if (!a || !b) {
        if (a) (psram ? Psram::free(a) : std::free(a));
        if (b) (psram ? Psram::free(b) : std::free(b));
        return false;
    }
    for (std::size_t i = 0; i < len_; ++i) { a[i] = 0; b[i] = 0; }
    buf_l_ = a; buf_r_ = b; owned_psram_ = psram;
    write_ = 0;
    return true;
}
StereoEnsemble::~StereoEnsemble() {
    if (buf_l_) (owned_psram_ ? Psram::free(buf_l_) : std::free(buf_l_));
    if (buf_r_) (owned_psram_ ? Psram::free(buf_r_) : std::free(buf_r_));
}
void StereoEnsemble::set_rate_hz (float hz) { rate_     = hz; }
void StereoEnsemble::set_depth_ms(float ms) { depth_ms_ = ms; }
void StereoEnsemble::set_mix     (float m)  { mix_      = m < 0 ? 0 : (m > 1 ? 1 : m); }
void StereoEnsemble::set_feedback(float f)  { fb_       = f < 0 ? 0 : (f > 0.7f ? 0.7f : f); }

void StereoEnsemble::process(const Real* in_l, const Real* in_r,
                             Real* out_l, Real* out_r, std::size_t frames) {
    if (!buf_l_ || !buf_r_) {
        for (std::size_t i = 0; i < frames; ++i) { out_l[i] = in_l[i]; out_r[i] = in_r[i]; }
        return;
    }
    // Three voices with detuned LFOs, fanned across the stereo field.
    constexpr float kVoiceRates[kVoices] = {1.00f, 1.07f, 0.93f};
    constexpr float kVoicePan [kVoices] = {-0.7f, 0.0f, +0.7f};

    const float depth_samp = depth_ms_ * 0.001f * sr_;
    const float base_samp  = depth_samp + 4.0f;        // never go below ~4 samples

    for (std::size_t i = 0; i < frames; ++i) {
        float xL = to_float(in_l[i]);
        float xR = to_float(in_r[i]);

        // Write input + feedback to both buffers.
        std::size_t wi = write_;
        buf_l_[wi] = from_float(xL + to_float(buf_l_[(wi + len_ - 1) % len_]) * fb_);
        buf_r_[wi] = from_float(xR + to_float(buf_r_[(wi + len_ - 1) % len_]) * fb_);

        float wet_l = 0, wet_r = 0;
        for (int v = 0; v < kVoices; ++v) {
            lfo_phase_[v] += rate_ * kVoiceRates[v] / sr_;
            if (lfo_phase_[v] >= 1.0f) lfo_phase_[v] -= 1.0f;
            float lfo = std::sin(2.0f * 3.14159265f * lfo_phase_[v]);
            float d   = base_samp + depth_samp * lfo;
            std::size_t da = (std::size_t)d;
            float       df = d - (float)da;
            std::size_t r0 = (wi + len_ - da)     % len_;
            std::size_t r1 = (wi + len_ - da - 1) % len_;

            float sL = (1.0f - df) * to_float(buf_l_[r0]) + df * to_float(buf_l_[r1]);
            float sR = (1.0f - df) * to_float(buf_r_[r0]) + df * to_float(buf_r_[r1]);
            float p  = kVoicePan[v];
            wet_l   += sL * (1.0f - 0.5f * (p > 0 ? p : 0));
            wet_r   += sR * (1.0f - 0.5f * (p < 0 ? -p : 0));
        }
        wet_l *= 1.0f / kVoices;
        wet_r *= 1.0f / kVoices;

        out_l[i] = from_float(xL * (1.0f - mix_) + wet_l * mix_);
        out_r[i] = from_float(xR * (1.0f - mix_) + wet_r * mix_);
        write_ = (write_ + 1) % len_;
    }
}

}  // namespace picoadk::dsp
