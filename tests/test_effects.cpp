// Effect smoke tests — ring mod, phaser, ensemble.

#include "picoadk/dsp/effects.h"
#include "picoadk/dsp/real.h"
#include <cassert>
#include <cmath>
#include <cstdio>

using namespace picoadk::dsp;
constexpr float kSR = 48000.0f;

void test_ring_mod_silences_dc_at_audio_carrier() {
    RingModulator r; r.reset(kSR); r.set_carrier_hz(440.0f); r.set_mix(1.0f);
    // DC × sine = sine; expect oscillating output.
    int crossings = 0;
    float prev = 0;
    for (int i = 0; i < 4800; ++i) {       // 100 ms — should see ~88 zero-crossings
        float y = to_float(r.process(from_float(0.5f)));
        if ((prev < 0 && y >= 0) || (prev > 0 && y <= 0)) ++crossings;
        prev = y;
    }
    printf("ring mod DC × 440 Hz crossings: %d (expect ≈ 88)\n", crossings);
    assert(crossings > 60 && crossings < 120);
}

void test_phaser_doesnt_explode() {
    Phaser p; p.reset(kSR); p.set_stages(4); p.set_rate_hz(0.5f); p.set_depth(0.8f);
    p.set_feedback(0.5f); p.set_mix(0.5f);
    float peak = 0;
    for (int i = 0; i < 24000; ++i) {
        float x = std::sin(2.0f * 3.14159f * 800.0f * i / kSR);
        float y = to_float(p.process(from_float(x)));
        if (i > 12000) peak = std::fmax(peak, std::fabs(y));
    }
    printf("phaser peak: %.3f (must be finite, < 5)\n", peak);
    assert(std::isfinite(peak));
    assert(peak < 5.0f);
}

void test_ensemble_passthrough_when_dry() {
    StereoEnsemble e;
    bool ok = e.configure(kSR, 30.0f);
    assert(ok);
    e.set_mix(0.0f);                       // pure dry
    Real lin[64], rin[64], lo[64], ro[64];
    for (int i = 0; i < 64; ++i) {
        lin[i] = from_float(0.5f);
        rin[i] = from_float(-0.5f);
    }
    e.process(lin, rin, lo, ro, 64);
    for (int i = 0; i < 64; ++i) {
        assert(std::fabs(to_float(lo[i]) - 0.5f)  < 1e-3f);
        assert(std::fabs(to_float(ro[i]) + 0.5f)  < 1e-3f);
    }
    printf("stereo ensemble dry path passthrough OK\n");
}

int main() {
    test_ring_mod_silences_dc_at_audio_carrier();
    test_phaser_doesnt_explode();
    test_ensemble_passthrough_when_dry();
    printf("effects tests OK\n");
    return 0;
}
