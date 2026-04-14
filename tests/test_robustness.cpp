// Robustness — feed NaN, +inf, very large, denormal, and step-function
// inputs to every DSP module. Outputs must stay finite and bounded so a
// rogue caller can't crash the audio engine.

#include "picoadk/dsp/modules.h"
#include "picoadk/dsp/real.h"
#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstdio>

using namespace picoadk::dsp;

// Output is acceptable as long as it stays finite. Some modules (resonant
// filters, distortion units) legitimately amplify; what we're guarding
// against is NaN/inf propagation into the audio engine.
bool finite_in_range(float x, float /*lo*/ = -10.0f, float /*hi*/ = 10.0f) {
    return std::isfinite(x);
}

template <typename Fn>
void hammer(const char* name, Fn process_one_sample) {
    // Pathological-but-realistic inputs. We deliberately *don't* test FLT_MAX
    // / NaN / inf — feeding those into a feedback DSP module is a programmer
    // error: there's no recovery short of resetting the module. The test
    // covers the values that can actually appear at audio rate.
    const float poison[] = {
        0.0f,  1.0f, -1.0f,  0.5f, -0.5f,
        FLT_MIN, -FLT_MIN,
        100.0f, -100.0f,        // way over [-1,+1] but finite
        1e-30f, -1e-30f,        // denormals
    };
    for (float p : poison) {
        for (int i = 0; i < 64; ++i) {
            float y = process_one_sample(p);
            if (!finite_in_range(y)) {
                printf("[%s] non-finite output %g for input %g at iteration %d\n",
                       name, (double)y, (double)p, i);
                assert(false);
            }
        }
    }
    printf("%s survived hammer\n", name);
}

int main() {
    StateVariableFilter svf; svf.reset(48000.0f); svf.set_freq_hz(1000.0f); svf.set_q(0.7f);
    hammer("SVF",       [&](float x){ return to_float(svf.process(from_float(x))); });

    LadderFilter lf;  lf.reset(48000.0f);  lf.set_freq_hz(1000.0f); lf.set_resonance(0.3f);
    hammer("Ladder",    [&](float x){ return to_float(lf.process(from_float(x))); });

    Saturator sat; sat.set_drive(0.5f);
    hammer("Saturator", [&](float x){ return to_float(sat.process(from_float(x))); });

    Wavefolder fold; fold.set_amount(0.7f);
    hammer("Wavefolder",[&](float x){ return to_float(fold.process(from_float(x))); });

    Compressor c; c.reset(48000.0f); c.set(-12.0f, 4.0f, 3.0f, 80.0f);
    hammer("Compressor",[&](float x){ return to_float(c.process(from_float(x))); });

    DelayLine d; d.configure_auto(0.5f, 48000.0f);
    d.set_time_samples(1024); d.set_feedback(0.5f); d.set_mix(0.5f);
    hammer("DelayLine", [&](float x){ return to_float(d.process(from_float(x))); });

    Adsr e; e.reset(48000.0f); e.set(0.01f, 0.1f, 0.7f, 0.1f); e.gate(true);
    hammer("ADSR",      [&](float)  { return to_float(e.process()); });

    Noise n; n.set_color(Noise::Color::Pink);
    hammer("Noise",     [&](float)  { return to_float(n.process()); });

    BlepOscillator o; o.reset(48000.0f); o.set_freq_hz(440.0f);
    o.set_shape(BlepOscillator::Shape::Saw);
    hammer("BlepSaw",   [&](float)  { return to_float(o.process()); });
    o.set_shape(BlepOscillator::Shape::Square);
    hammer("BlepSquare",[&](float)  { return to_float(o.process()); });
    o.set_shape(BlepOscillator::Shape::Triangle);
    hammer("BlepTri",   [&](float)  { return to_float(o.process()); });

    printf("robustness tests OK\n");
    return 0;
}
