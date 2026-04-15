// Filter zoo — verify the new filter models settle correctly and don't
// blow up under sustained input.

#include "picoadk/dsp/filters.h"
#include "picoadk/dsp/real.h"
#include <cassert>
#include <cmath>
#include <cstdio>

using namespace picoadk::dsp;
constexpr float kSR = 48000.0f;

void test_one_pole_lp_passes_dc() {
    OnePole p; p.reset(kSR); p.set_mode(OnePole::Mode::LowPass); p.set_freq_hz(2000.0f);
    float last = 0;
    for (int i = 0; i < 4096; ++i) last = to_float(p.process(from_float(1.0f)));
    printf("one-pole LP DC: %.4f (≈ 1.0)\n", last);
    assert(std::fabs(last - 1.0f) < 0.05f);
}

void test_one_pole_hp_blocks_dc() {
    OnePole p; p.reset(kSR); p.set_mode(OnePole::Mode::HighPass); p.set_freq_hz(200.0f);
    float last = 0;
    for (int i = 0; i < 8192; ++i) last = to_float(p.process(from_float(1.0f)));
    printf("one-pole HP DC: %.4f (≈ 0.0)\n", last);
    assert(std::fabs(last) < 0.05f);
}

void test_biquad_lowpass_passes_dc() {
    Biquad b; b.reset(kSR); b.set_lowpass(2000.0f, 0.7f);
    float last = 0;
    for (int i = 0; i < 4096; ++i) last = to_float(b.process(from_float(1.0f)));
    printf("biquad LP DC: %.4f (≈ 1.0)\n", last);
    assert(std::fabs(last - 1.0f) < 0.05f);
}

void test_biquad_highpass_blocks_dc() {
    Biquad b; b.reset(kSR); b.set_highpass(200.0f, 0.7f);
    float last = 0;
    for (int i = 0; i < 8192; ++i) last = to_float(b.process(from_float(1.0f)));
    printf("biquad HP DC: %.4f (≈ 0.0)\n", last);
    assert(std::fabs(last) < 0.05f);
}

void test_biquad_peak_boost() {
    Biquad b; b.reset(kSR); b.set_peak(1000.0f, 1.0f, +12.0f);
    // Drive with sine at peak frequency, expect amplitude boost.
    float peak = 0;
    for (int i = 0; i < 8192; ++i) {
        float x = std::sin(2.0f * 3.14159f * 1000.0f * i / kSR);
        float y = to_float(b.process(from_float(x)));
        if (i > 4000) peak = std::fmax(peak, std::fabs(y));
    }
    printf("biquad peak +12 dB at center: %.3f (≈ 4.0)\n", peak);
    assert(peak > 2.5f && peak < 5.0f);
}

void test_moog_zdf_settles() {
    MoogLadderZdf m; m.reset(kSR); m.set_freq_hz(800.0f); m.set_resonance(0.3f);
    float last = 0;
    for (int i = 0; i < 8192; ++i) last = to_float(m.process(from_float(0.5f)));
    printf("Moog ZDF DC: %.4f (0..0.5)\n", last);
    assert(last > 0.0f && last < 0.6f);
}

void test_dc_blocker() {
    DcBlocker d;
    float last = 0;
    for (int i = 0; i < 8192; ++i) last = to_float(d.process(from_float(0.5f)));
    printf("DC blocker output for DC=0.5: %.4f (≈ 0)\n", last);
    assert(std::fabs(last) < 0.01f);
}

void test_all_pass_unit_magnitude() {
    AllPass1 ap; ap.set_coefficient(0.7f);
    // RMS of output should be close to RMS of input (allpass = unit magnitude).
    double in_e = 0, out_e = 0;
    for (int i = 0; i < 8192; ++i) {
        float x = std::sin(2.0f * 3.14159f * 1000.0f * i / kSR);
        float y = to_float(ap.process(from_float(x)));
        in_e  += x * x;
        out_e += y * y;
    }
    printf("allpass energy ratio out/in: %.3f (≈ 1.0)\n", out_e / in_e);
    assert(out_e > 0.9 * in_e && out_e < 1.1 * in_e);
}

int main() {
    test_one_pole_lp_passes_dc();
    test_one_pole_hp_blocks_dc();
    test_biquad_lowpass_passes_dc();
    test_biquad_highpass_blocks_dc();
    test_biquad_peak_boost();
    test_moog_zdf_settles();
    test_dc_blocker();
    test_all_pass_unit_magnitude();
    printf("more_filters tests OK\n");
    return 0;
}
