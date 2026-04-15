// Analog-flavoured audio effects.
//
// Each effect is self-contained, allocates its own state (PSRAM-backed when
// available for the ones that need a delay buffer), and exposes a small set
// of musical knobs. Stack them with the other dsp:: modules in any order;
// the audio callback contract is `process(out_l, out_r, frames)` for
// stereo-aware effects, or `process(x)` per sample for mono.

#pragma once

#include <cstddef>
#include <cstdint>

#include "picoadk/dsp/real.h"
#include "picoadk/dsp/filters.h"

namespace picoadk::dsp {

// =============================================================================
// Ring modulator — multiplies the input by an internal sine carrier.
//
//     y[n] = x[n] * sin(2*pi * fc * n / sr)
//
// Bell-tone / metallic timbres at musical fc, classic dalek voice at low
// fc. Through-zero by setting carrier_hz_ negative.
// =============================================================================
class RingModulator {
public:
    void  reset(float sr);
    void  set_carrier_hz(float hz);
    void  set_mix(float dry_wet);            // 0 = dry, 1 = wet
    Real  process(Real x);

private:
    float sr_     = 48000.0f;
    float phase_  = 0.0f;
    float inc_    = 0.0f;
    float mix_    = 1.0f;
};

// =============================================================================
// Phaser — N-stage all-pass with a single shared LFO. Classic 4- or
// 6-stage analogue phaser texture; depth controls LFO sweep range,
// feedback adds resonance.
// =============================================================================
class Phaser {
public:
    static constexpr int kMaxStages = 8;

    void  reset(float sr);
    void  set_stages(int n);                 // 2..kMaxStages, even numbers musical
    void  set_rate_hz(float lfo_hz);
    void  set_depth(float depth);            // 0..1
    void  set_feedback(float fb);            // 0..0.95
    void  set_mix(float dry_wet);            // 0..1
    Real  process(Real x);

private:
    float    sr_         = 48000.0f;
    int      stages_     = 4;
    float    lfo_phase_  = 0.0f;
    float    lfo_inc_    = 0.0f;
    float    depth_      = 0.5f;
    float    fb_         = 0.0f;
    float    mix_        = 0.5f;
    float    fb_state_   = 0.0f;
    AllPass1 ap_[kMaxStages];
};

// =============================================================================
// Stereo ensemble / chorus — three modulated delay taps fanned across the
// stereo field. Different LFO rates per voice → "string ensemble"-style
// shimmer; rate=0 → static comb (bring-your-own LFO).
//
// Buffer is owned by the effect; configure() picks PSRAM when available.
// 30 ms max delay covers from chorus to flange to short slap.
// =============================================================================
class StereoEnsemble {
public:
    bool  configure(float sample_rate, float max_delay_ms = 30.0f);
    ~StereoEnsemble();

    void  set_rate_hz(float hz);             // base LFO rate
    void  set_depth_ms(float ms);            // sweep amount
    void  set_mix(float dry_wet);            // 0..1
    void  set_feedback(float fb);            // 0..0.7 — flanger territory above 0.4

    void  process(const Real* in_l, const Real* in_r,
                  Real* out_l,       Real* out_r, std::size_t frames);

private:
    static constexpr int kVoices = 3;
    Real*       buf_l_    = nullptr;
    Real*       buf_r_    = nullptr;
    bool        owned_psram_ = false;
    std::size_t len_      = 0;
    std::size_t write_    = 0;
    float       sr_       = 48000.0f;
    float       rate_     = 0.5f;
    float       depth_ms_ = 5.0f;
    float       mix_      = 0.5f;
    float       fb_       = 0.0f;
    float       lfo_phase_[kVoices] = {0.0f, 0.33f, 0.66f};
};

}  // namespace picoadk::dsp
