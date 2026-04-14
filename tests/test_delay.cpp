// Delay line — write a single impulse, expect to read it back N samples later.

#include "picoadk/dsp/modules.h"
#include "picoadk/dsp/real.h"
#include <cassert>
#include <cmath>
#include <cstdio>

using namespace picoadk::dsp;

int main() {
    DelayLine d;
    bool ok = d.configure_auto(/*max sec*/0.5f, 48000.0f);
    assert(ok);
    d.set_time_samples(1024);
    d.set_feedback(0.0f);
    d.set_mix(1.0f);

    // Send a 1.0 impulse, then 0s.
    float first = to_float(d.process(from_float(1.0f)));
    (void)first;
    float echo = 0.0f;
    for (int i = 0; i < 1024; ++i) {
        float s = to_float(d.process(from_float(0.0f)));
        if (i == 1023) echo = s;
    }
    printf("DelayLine echo at +1024: %.4f (expect ≈ 1.0)\n", echo);
    assert(std::fabs(echo - 1.0f) < 0.05f);
    printf("delay tests OK\n");
    return 0;
}
