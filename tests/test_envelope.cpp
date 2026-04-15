// ADSR envelope — gate on rises towards 1, gate off falls towards 0.

#include "picoadk/dsp/modules.h"
#include "picoadk/dsp/real.h"
#include <cassert>
#include <cmath>
#include <cstdio>

using namespace picoadk::dsp;

int main() {
    Adsr e;
    e.reset(48000.0f);
    e.set(/*a*/0.01f, /*d*/0.05f, /*s*/0.6f, /*r*/0.1f);
    e.gate(true);

    float peak = 0;
    for (int i = 0; i < 48000; ++i) {       // 1 s with gate on
        float v = to_float(e.process());
        if (v > peak) peak = v;
    }
    printf("ADSR sustain peak: %.3f (target ≈ 0.6)\n", peak);
    assert(peak > 0.4f && peak < 1.05f);

    e.gate(false);
    float final_v = 0;
    for (int i = 0; i < 48000; ++i) final_v = to_float(e.process());
    printf("ADSR after release: %.4f\n", final_v);
    assert(final_v < 0.05f);
    assert(!e.active());

    printf("envelope tests OK\n");
    return 0;
}
