// Smoke tests for oscillators — verify they produce non-silence and the
// fundamental frequency is approximately right.

#include "picoadk/dsp/modules.h"
#include "picoadk/dsp/real.h"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <vector>

using namespace picoadk::dsp;

constexpr float kSR = 48000.0f;

// Estimate fundamental by counting sign changes (very rough, fine for
// a smoke test on a stable waveform).
float estimate_freq(const std::vector<float>& buf) {
    int crossings = 0;
    for (std::size_t i = 1; i < buf.size(); ++i) {
        if ((buf[i - 1] < 0 && buf[i] >= 0) || (buf[i - 1] > 0 && buf[i] <= 0))
            ++crossings;
    }
    float seconds = (float)buf.size() / kSR;
    return crossings * 0.5f / seconds;
}

void test_blep_saw() {
    BlepOscillator o;
    o.reset(kSR);
    o.set_shape(BlepOscillator::Shape::Saw);
    o.set_freq_hz(440.0f);
    std::vector<float> buf(kSR / 4);   // 250 ms
    for (auto& s : buf) s = to_float(o.process());

    // Non-silence
    float peak = 0;
    for (auto s : buf) peak = std::fmax(peak, std::fabs(s));
    assert(peak > 0.5f);

    // Fundamental within 5 %
    float f = estimate_freq(buf);
    printf("blep_saw fundamental: %.1f Hz (target 440)\n", f);
    assert(std::fabs(f - 440.0f) < 22.0f);
}

void test_blep_square() {
    BlepOscillator o;
    o.reset(kSR);
    o.set_shape(BlepOscillator::Shape::Square);
    o.set_freq_hz(220.0f);
    std::vector<float> buf(kSR / 4);
    for (auto& s : buf) s = to_float(o.process());

    float peak = 0;
    for (auto s : buf) peak = std::fmax(peak, std::fabs(s));
    assert(peak > 0.5f);

    float f = estimate_freq(buf);
    printf("blep_square fundamental: %.1f Hz (target 220)\n", f);
    assert(std::fabs(f - 220.0f) < 11.0f);
}

void test_two_instances_independent() {
    // Regression test for the triangle-state-as-static-thread_local bug.
    BlepOscillator a, b;
    a.reset(kSR); a.set_shape(BlepOscillator::Shape::Triangle); a.set_freq_hz(440.0f);
    b.reset(kSR); b.set_shape(BlepOscillator::Shape::Triangle); b.set_freq_hz(110.0f);
    for (int i = 0; i < 1000; ++i) { (void)a.process(); (void)b.process(); }
    // Different frequencies should give different last-frame values.
    float fa = to_float(a.process());
    float fb = to_float(b.process());
    printf("two-instance check: a=%.3f  b=%.3f\n", fa, fb);
    assert(std::fabs(fa - fb) > 1e-6f);
}

int main() {
    test_blep_saw();
    test_blep_square();
    test_two_instances_independent();
    printf("oscillator tests OK\n");
    return 0;
}
