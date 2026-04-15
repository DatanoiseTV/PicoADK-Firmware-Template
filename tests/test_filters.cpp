// Filter sanity checks — DC passes through LP, gets blocked by HP.

#include "picoadk/dsp/modules.h"
#include "picoadk/dsp/real.h"
#include <cassert>
#include <cmath>
#include <cstdio>

using namespace picoadk::dsp;

constexpr float kSR = 48000.0f;

void test_svf_lowpass_passes_dc() {
    StateVariableFilter f;
    f.reset(kSR);
    f.set_mode(StateVariableFilter::Mode::LP);
    f.set_freq_hz(2000.0f);
    f.set_q(0.7f);

    float last = 0;
    for (int i = 0; i < 4096; ++i) last = to_float(f.process(from_float(1.0f)));
    printf("SVF LP DC output: %.4f (expect ≈ 1.0)\n", last);
    assert(std::fabs(last - 1.0f) < 0.05f);
}

void test_svf_highpass_blocks_dc() {
    StateVariableFilter f;
    f.reset(kSR);
    f.set_mode(StateVariableFilter::Mode::HP);
    f.set_freq_hz(200.0f);
    f.set_q(0.7f);

    float last = 0;
    for (int i = 0; i < 8192; ++i) last = to_float(f.process(from_float(1.0f)));
    printf("SVF HP DC output: %.4f (expect ≈ 0.0)\n", last);
    assert(std::fabs(last) < 0.05f);
}

void test_ladder_settles() {
    LadderFilter f;
    f.reset(kSR);
    f.set_freq_hz(1000.0f);
    f.set_resonance(0.2f);

    float last = 0;
    for (int i = 0; i < 8192; ++i) last = to_float(f.process(from_float(0.5f)));
    printf("Ladder DC output: %.4f (expect 0..0.5)\n", last);
    assert(last > 0.0f && last < 0.6f);
}

int main() {
    test_svf_lowpass_passes_dc();
    test_svf_highpass_blocks_dc();
    test_ladder_settles();
    printf("filter tests OK\n");
    return 0;
}
