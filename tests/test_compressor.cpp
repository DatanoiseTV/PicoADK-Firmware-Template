// Compressor — above threshold, gain reduction must kick in. Below it,
// the signal passes through close to unity.

#include "picoadk/dsp/modules.h"
#include "picoadk/dsp/real.h"
#include <cassert>
#include <cmath>
#include <cstdio>

using namespace picoadk::dsp;

float run_steady(Compressor& c, float input_amp, std::size_t samples) {
    float peak = 0;
    for (std::size_t i = 0; i < samples; ++i) {
        float s = (i & 1) ? input_amp : -input_amp;          // square wave keeps env tracking
        float y = to_float(c.process(from_float(s)));
        if (i > samples / 2) peak = std::fmax(peak, std::fabs(y));
    }
    return peak;
}

void test_below_threshold_is_unity() {
    Compressor c;
    c.reset(48000.0f);
    c.set(/*thr_db*/-12.0f, /*ratio*/4.0f, /*atk_ms*/3.0f, /*rel_ms*/80.0f);
    float out = run_steady(c, 0.10f, 8000);    // -20 dB ≈ below thr
    printf("below-threshold peak: %.3f (input 0.10)\n", out);
    assert(out > 0.08f && out < 0.13f);
}

void test_above_threshold_compresses() {
    Compressor c;
    c.reset(48000.0f);
    c.set(/*thr_db*/-12.0f, /*ratio*/8.0f, /*atk_ms*/3.0f, /*rel_ms*/80.0f);
    // Drive a big signal — well above -12 dB threshold.
    float out = run_steady(c, 0.90f, 24000);
    printf("above-threshold peak: %.3f (input 0.90)\n", out);
    // 8:1 ratio should pull the steady-state output well below the input.
    assert(out < 0.7f);
    assert(out > 0.0f);
}

int main() {
    test_below_threshold_is_unity();
    test_above_threshold_compresses();
    printf("compressor tests OK\n");
    return 0;
}
