// =============================================================================
//  PicoADK v3 — FX Rack
// -----------------------------------------------------------------------------
//  Full-duplex stereo passthrough with a delay and a Hadamard FDN reverb in
//  parallel. The four MCP3208 pots control:
//
//     pot 0 → delay time     (0 .. 1.5 s)
//     pot 1 → delay feedback (0 .. 0.95)
//     pot 2 → reverb size    (0 .. 1)
//     pot 3 → wet/dry mix    (0 .. 1)
//
//  On v2 the delay and reverb buffers automatically allocate from PSRAM —
//  no SRAM blown for 3.5 s of impulse response.
// =============================================================================

#include "picoadk/picoadk.h"

#include <FreeRTOS.h>
#include <task.h>

using namespace picoadk;
using namespace picoadk::dsp;

namespace {

constexpr float kSampleRate = 48000.0f;

DelayLine  g_delay_l, g_delay_r;
FdnReverb  g_verb;

void audio_cb(const float* const* in, float* const* out, std::size_t frames, void*) {
    if (!in) {
        for (std::size_t i = 0; i < frames; ++i) out[0][i] = out[1][i] = 0;
        return;
    }
    float pot_time = Controls::read_adc8_norm(0);     // 0..1
    float pot_fb   = Controls::read_adc8_norm(1);
    float pot_size = Controls::read_adc8_norm(2);
    float pot_mix  = Controls::read_adc8_norm(3);

    g_delay_l.set_time_seconds(pot_time * 1.5f, kSampleRate);
    g_delay_r.set_time_seconds(pot_time * 1.5f * 0.97f, kSampleRate);
    g_delay_l.set_feedback(pot_fb * 0.95f);
    g_delay_r.set_feedback(pot_fb * 0.95f);
    g_delay_l.set_mix(1.0f);     // we mix dry/wet ourselves below
    g_delay_r.set_mix(1.0f);
    g_verb.set_size(pot_size);
    g_verb.set_damping(0.5f);

    // Reverb processes the whole block at once.
    float verb_l[64], verb_r[64];
    g_verb.process(in[0], in[1], verb_l, verb_r, frames);

    for (std::size_t i = 0; i < frames; ++i) {
        float dl = to_float(g_delay_l.process(from_float(in[0][i])));
        float dr = to_float(g_delay_r.process(from_float(in[1][i])));
        float wet_l = 0.5f * (dl + to_float(verb_l[i]));
        float wet_r = 0.5f * (dr + to_float(verb_r[i]));
        out[0][i] = in[0][i] * (1.0f - pot_mix) + wet_l * pot_mix;
        out[1][i] = in[1][i] * (1.0f - pot_mix) + wet_r * pot_mix;
    }
}

[[noreturn]] void watchdog_task(void*) {
    for (;;) { System::feed_watchdog(); vTaskDelay(pdMS_TO_TICKS(500)); }
}

}  // namespace

int main() {
    HardwareConfig hw;
    hw.audio.sample_rate_hz = (uint32_t)kSampleRate;
    hw.audio.block_size     = 64;
    hw.audio.direction      = AudioDirection::Duplex;
    Hardware::init(hw);

    g_delay_l.configure_auto(1.5f, kSampleRate);
    g_delay_r.configure_auto(1.5f, kSampleRate);
    g_verb.configure(3.5f, kSampleRate);

    Audio::set_callback(audio_cb);
    Audio::start();

    xTaskCreate(watchdog_task, "WDOG", 256, nullptr, tskIDLE_PRIORITY + 1, nullptr);
    vTaskStartScheduler();
    for (;;) __asm volatile("wfi");
}
