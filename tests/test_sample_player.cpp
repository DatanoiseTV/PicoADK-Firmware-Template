// SamplePlayer — pitch shift and loop behaviour exercised against a
// synthetic in-memory SampleSource (no SD needed).

#include "picoadk/dsp/sample_player.h"
#include "picoadk/dsp/real.h"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <vector>

using namespace picoadk::dsp;

class SineSource final : public SampleSource {
public:
    SineSource(float freq_hz, std::size_t length, std::size_t sr)
        : len_(length), sr_(sr) {
        data_l_.resize(length);
        data_r_.resize(length);
        for (std::size_t i = 0; i < length; ++i) {
            float v = std::sin(2.0f * 3.14159265f * freq_hz * (float)i / (float)sr);
            data_l_[i] = from_float(v);
            data_r_[i] = from_float(v);
        }
    }
    std::size_t channels()       const override { return 2; }
    std::size_t length()         const override { return len_; }
    std::size_t sample_rate_hz() const override { return sr_; }
    std::size_t read(std::size_t pos, Real** out, std::size_t frames) override {
        if (pos >= len_) return 0;
        std::size_t n = std::min(frames, len_ - pos);
        for (std::size_t i = 0; i < n; ++i) {
            out[0][i] = data_l_[pos + i];
            out[1][i] = data_r_[pos + i];
        }
        return n;
    }
private:
    std::vector<Real> data_l_, data_r_;
    std::size_t       len_;
    std::size_t       sr_;
};

float estimate_freq(const std::vector<float>& buf, float sr) {
    int crossings = 0;
    for (std::size_t i = 1; i < buf.size(); ++i)
        if ((buf[i - 1] < 0 && buf[i] >= 0) || (buf[i - 1] > 0 && buf[i] <= 0))
            ++crossings;
    return crossings * 0.5f * sr / (float)buf.size();
}

void test_native_speed_passes_through() {
    SineSource src(440.0f, 48000, 48000);
    SamplePlayer p; p.reset(48000.0f); p.set_source(&src); p.trigger();
    std::vector<float> out_l(8192, 0.0f), out_r(8192, 0.0f);
    Real* lbuf = (Real*)alloca(8192 * sizeof(Real));
    Real* rbuf = (Real*)alloca(8192 * sizeof(Real));
    for (std::size_t i = 0; i < 8192; ++i) lbuf[i] = rbuf[i] = 0;
    p.process(lbuf, rbuf, 8192);
    for (std::size_t i = 0; i < 8192; ++i) out_l[i] = to_float(lbuf[i]);
    float f = estimate_freq(out_l, 48000.0f);
    printf("SamplePlayer 1.0× speed → %.1f Hz (target 440)\n", f);
    assert(std::fabs(f - 440.0f) < 25.0f);
}

void test_pitch_shift_octave_up() {
    SineSource src(440.0f, 48000, 48000);
    SamplePlayer p; p.reset(48000.0f); p.set_source(&src);
    p.set_pitch_semitones(+12.0f);
    p.trigger();
    std::vector<float> out_l(8192, 0.0f);
    Real* lbuf = (Real*)alloca(8192 * sizeof(Real));
    Real* rbuf = (Real*)alloca(8192 * sizeof(Real));
    for (std::size_t i = 0; i < 8192; ++i) lbuf[i] = rbuf[i] = 0;
    p.process(lbuf, rbuf, 8192);
    for (std::size_t i = 0; i < 8192; ++i) out_l[i] = to_float(lbuf[i]);
    float f = estimate_freq(out_l, 48000.0f);
    printf("SamplePlayer +12 semis → %.1f Hz (target 880)\n", f);
    assert(std::fabs(f - 880.0f) < 50.0f);
}

void test_loop_keeps_playing() {
    // 1000-sample source, looped — should still produce output well past length.
    SineSource src(880.0f, 1000, 48000);
    SamplePlayer p; p.reset(48000.0f); p.set_source(&src);
    p.set_loop(true, 0, 1000);
    p.trigger();
    Real* lbuf = (Real*)alloca(8000 * sizeof(Real));
    Real* rbuf = (Real*)alloca(8000 * sizeof(Real));
    for (std::size_t i = 0; i < 8000; ++i) lbuf[i] = rbuf[i] = 0;
    p.process(lbuf, rbuf, 8000);
    float peak = 0;
    for (std::size_t i = 5000; i < 8000; ++i) peak = std::fmax(peak, std::fabs(to_float(lbuf[i])));
    printf("looped sample peak past length: %.3f (expect ≈ 1.0)\n", peak);
    assert(peak > 0.5f);
    assert(p.active());
}

int main() {
    test_native_speed_passes_through();
    test_pitch_shift_octave_up();
    test_loop_keeps_playing();
    printf("sample_player tests OK\n");
    return 0;
}
