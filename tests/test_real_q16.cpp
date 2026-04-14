// Q16 fixed-point Real type — when compiled with PICOADK_BOARD_SELECT_V1
// the dsp::Real conversions and operators map to the libfixmath-derived
// path. Verify the round-trips and basic ops are accurate enough.
//
// This test is built TWICE — once for the host (float Real) and once with
// PICOADK_BOARD_SELECT_V1 forced (Q16 Real) — see tests/CMakeLists.txt.

#include "picoadk/dsp/real.h"
#include "picoadk/dsp/math.h"
#include <cassert>
#include <cmath>
#include <cstdio>

using namespace picoadk::dsp;

void test_round_trip_floats() {
    const float vals[] = {0.0f, 0.25f, -0.25f, 0.99f, -0.99f, 0.001f, -0.001f};
    for (float f : vals) {
        Real r = from_float(f);
        float back = to_float(r);
        printf("round-trip %g → %g\n", (double)f, (double)back);
        assert(std::fabs(back - f) < 1e-3f);
    }
}

void test_clip() {
    assert(to_float(clip(from_float( 0.5f))) ==  0.5f);
    assert(to_float(clip(from_float( 2.0f))) <=  1.0f);
    assert(to_float(clip(from_float(-2.0f))) >= -1.0f);
}

void test_mul_mac() {
    // 0.5 * 0.5 = 0.25
    Real a = from_float(0.5f), b = from_float(0.5f);
    float r = to_float(mul(a, b));
    printf("mul(0.5, 0.5) = %g (expect 0.25)\n", (double)r);
    assert(std::fabs(r - 0.25f) < 1e-2f);

    Real acc = from_float(0.10f);
    acc = mac(acc, a, b);
    printf("mac(0.10, 0.5, 0.5) = %g (expect 0.35)\n", (double)to_float(acc));
    assert(std::fabs(to_float(acc) - 0.35f) < 1e-2f);
}

int main() {
    test_round_trip_floats();
    test_clip();
    test_mul_mac();
    printf("real/q16 tests OK\n");
    return 0;
}
