// Filter implementations.
//
// Math is written in floats; on RP2350 the M33 FPU evaluates it directly,
// on RP2040 the compiler maps to soft-float (slow) — except in hot inner
// loops where we drop into the dsp::math helpers (mul/mac/qadd_sat/smlad_q15)
// which choose the fastest path per board.

#include "picoadk/dsp/filters.h"
#include "picoadk/dsp/math.h"
#include "picoadk/dsp/real.h"

#include <cmath>
#include <cstdint>

namespace picoadk::dsp {

// ---- OnePole (TPT) -------------------------------------------------------
void OnePole::reset(float sr) {
    sr_ = sr;
    z1_ = 0.0f;
    set_freq_hz(1000.0f);
}

void OnePole::set_freq_hz(float hz) {
    // TPT one-pole: g = tan(pi * fc / sr) / (1 + tan(...))
    float wd = 3.14159265f * hz / sr_;
    if (wd < 0)        wd = 0;
    if (wd > 1.555f)   wd = 1.555f;     // avoid tan blow-up near pi/2
    float wa = std::tan(wd);
    g_ = wa / (1.0f + wa);
}

Real OnePole::process(Real x_in) {
    float x  = to_float(x_in);
    float v  = (x - z1_) * g_;
    float lp = v + z1_;
    z1_ = lp + v;        // state update
    float out = (mode_ == Mode::LowPass) ? lp : (x - lp);
    return from_float(out);
}

// ---- Biquad (DF-II transposed) -------------------------------------------
void Biquad::reset(float sr) { sr_ = sr; z1_ = z2_ = 0.0f; }

void Biquad::set_coefficients(float b0, float b1, float b2, float a1, float a2) {
    b0_ = b0; b1_ = b1; b2_ = b2; a1_ = a1; a2_ = a2;
}

namespace {
struct RbjCoeffs {
    float b0, b1, b2, a0, a1, a2;
    void normalise() {
        b0 /= a0; b1 /= a0; b2 /= a0;
        a1 /= a0; a2 /= a0; a0 = 1.0f;
    }
};

void rbj_common(float fc, float q, float sr, float& sn, float& cs, float& alpha) {
    float w0 = 2.0f * 3.14159265f * fc / sr;
    sn       = std::sin(w0);
    cs       = std::cos(w0);
    alpha    = sn / (2.0f * (q > 0.0001f ? q : 0.0001f));
}
}  // anonymous

void Biquad::set_lowpass(float fc, float q) {
    float sn, cs, a; rbj_common(fc, q, sr_, sn, cs, a);
    RbjCoeffs c{ (1 - cs) * 0.5f, 1 - cs, (1 - cs) * 0.5f, 1 + a, -2 * cs, 1 - a };
    c.normalise();
    set_coefficients(c.b0, c.b1, c.b2, c.a1, c.a2);
}
void Biquad::set_highpass(float fc, float q) {
    float sn, cs, a; rbj_common(fc, q, sr_, sn, cs, a);
    RbjCoeffs c{ (1 + cs) * 0.5f, -(1 + cs), (1 + cs) * 0.5f, 1 + a, -2 * cs, 1 - a };
    c.normalise();
    set_coefficients(c.b0, c.b1, c.b2, c.a1, c.a2);
}
void Biquad::set_bandpass(float fc, float q) {
    float sn, cs, a; rbj_common(fc, q, sr_, sn, cs, a);
    RbjCoeffs c{ a, 0, -a, 1 + a, -2 * cs, 1 - a };
    c.normalise();
    set_coefficients(c.b0, c.b1, c.b2, c.a1, c.a2);
}
void Biquad::set_notch(float fc, float q) {
    float sn, cs, a; rbj_common(fc, q, sr_, sn, cs, a);
    RbjCoeffs c{ 1, -2 * cs, 1, 1 + a, -2 * cs, 1 - a };
    c.normalise();
    set_coefficients(c.b0, c.b1, c.b2, c.a1, c.a2);
}
void Biquad::set_peak(float fc, float q, float gain_db) {
    float A = std::pow(10.0f, gain_db / 40.0f);
    float sn, cs, a; rbj_common(fc, q, sr_, sn, cs, a);
    RbjCoeffs c{ 1 + a * A, -2 * cs, 1 - a * A, 1 + a / A, -2 * cs, 1 - a / A };
    c.normalise();
    set_coefficients(c.b0, c.b1, c.b2, c.a1, c.a2);
}
void Biquad::set_lowshelf(float fc, float /*q*/, float gain_db) {
    float A = std::pow(10.0f, gain_db / 40.0f);
    float w0 = 2.0f * 3.14159265f * fc / sr_;
    float sn = std::sin(w0), cs = std::cos(w0);
    float beta = std::sqrt(A) / 0.707f;
    RbjCoeffs c{
        A * ((A + 1) - (A - 1) * cs + beta * sn),
        2 * A * ((A - 1) - (A + 1) * cs),
        A * ((A + 1) - (A - 1) * cs - beta * sn),
        (A + 1) + (A - 1) * cs + beta * sn,
        -2 * ((A - 1) + (A + 1) * cs),
        (A + 1) + (A - 1) * cs - beta * sn,
    };
    c.normalise();
    set_coefficients(c.b0, c.b1, c.b2, c.a1, c.a2);
}
void Biquad::set_highshelf(float fc, float /*q*/, float gain_db) {
    float A = std::pow(10.0f, gain_db / 40.0f);
    float w0 = 2.0f * 3.14159265f * fc / sr_;
    float sn = std::sin(w0), cs = std::cos(w0);
    float beta = std::sqrt(A) / 0.707f;
    RbjCoeffs c{
        A * ((A + 1) + (A - 1) * cs + beta * sn),
        -2 * A * ((A - 1) + (A + 1) * cs),
        A * ((A + 1) + (A - 1) * cs - beta * sn),
        (A + 1) - (A - 1) * cs + beta * sn,
        2 * ((A - 1) - (A + 1) * cs),
        (A + 1) - (A - 1) * cs - beta * sn,
    };
    c.normalise();
    set_coefficients(c.b0, c.b1, c.b2, c.a1, c.a2);
}

Real Biquad::process(Real x_in) {
    // DF-II transposed: hot loop. Compiler emits VFMA on M33 and
    // soft-float MUL on M0+. For Q15-quantised coefficients an SMLAD-based
    // path can be ~2× faster on M33 — wire it in via dsp::smlad_q15 if
    // your filter coefficients come pre-quantised.
    float x = to_float(x_in);
    float y = b0_ * x + z1_;
    z1_     = b1_ * x - a1_ * y + z2_;
    z2_     = b2_ * x - a2_ * y;
    return from_float(y);
}

// ---- Moog ZDF ladder -----------------------------------------------------
void MoogLadderZdf::reset(float sr) {
    sr_ = sr;
    for (auto& s : s_) s = 0.0f;
    set_freq_hz(fc_);
}
void MoogLadderZdf::set_freq_hz(float hz) {
    fc_ = hz;
    float wd = 3.14159265f * hz / sr_;
    if (wd > 1.555f) wd = 1.555f;
    g_ = std::tan(wd);
    G_ = g_ / (1.0f + g_);
}
void MoogLadderZdf::set_resonance(float r) {
    res_ = r > 0 ? (r < 1.05f ? r : 1.05f) : 0;
}
Real MoogLadderZdf::process(Real x_in) {
    // Zavalishin's "The Art of VA Filter Design" section 5.5 — 4-pole ladder
    // implicit resolution.
    float x = to_float(x_in);
    float G2 = G_ * G_;
    float G3 = G2 * G_;
    float G4 = G3 * G_;
    float S  = G3 * s_[0] + G2 * s_[1] + G_ * s_[2] + s_[3];
    float u  = (x - 4.0f * res_ * S) / (1.0f + 4.0f * res_ * G4);

    auto step = [&](float& s, float in) {
        float v = (in - s) * G_;
        float lp = v + s;
        s = lp + v;
        return lp;
    };
    float v0 = step(s_[0], std::tanh(u));     // soft-clip in front (saturator)
    float v1 = step(s_[1], v0);
    float v2 = step(s_[2], v1);
    float v3 = step(s_[3], v2);
    return from_float(v3);
}

// ---- DC blocker ----------------------------------------------------------
Real DcBlocker::process(Real x_in) {
    float x = to_float(x_in);
    float y = x - x1_ + 0.995f * y1_;
    x1_ = x;
    y1_ = y;
    return from_float(y);
}

// ---- AllPass1 ------------------------------------------------------------
Real AllPass1::process(Real x_in) {
    float x  = to_float(x_in);
    float y  = -a_ * x + z1_;
    z1_      = x + a_ * y;
    return from_float(y);
}

// ---- Comb (Karplus-Strong / chorus building block) -----------------------
void Comb::configure(Real* buf, std::size_t len) {
    buf_ = buf; len_ = len; write_ = 0;
    for (std::size_t i = 0; i < len; ++i) buf[i] = 0;
}
void Comb::set_delay_samples(std::size_t s) { delay_ = (s < len_) ? s : len_ - 1; }
Real Comb::process(Real x_in) {
    if (!buf_ || !len_) return x_in;
    std::size_t r = (write_ + len_ - delay_) % len_;
    float wet = to_float(buf_[r]);
    if (damp_ > 0.0f) {
        lp_z1_ = lp_z1_ + (1.0f - damp_) * (wet - lp_z1_);
        wet = lp_z1_;
    }
    float in_y = to_float(x_in) + wet * fb_;
    buf_[write_] = from_float(in_y);
    write_ = (write_ + 1) % len_;
    return from_float(wet);
}

}  // namespace picoadk::dsp
