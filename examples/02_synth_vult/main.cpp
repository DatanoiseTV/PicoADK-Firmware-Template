// PicoADK v3 — Vult monosynth example.
//
// Mirrors the behaviour of the legacy src/main.cpp but built on the v3 HAL:
//   * Hardware::init()              — clocks, USB, MIDI, audio, controls
//   * Audio::set_callback(audio_cb) — float planar callback at the configured rate
//   * Midi::set_*()                 — single set of callbacks regardless of transport
//
// The DSP is exactly the same vultsrc/dsp.vult file the v1 firmware shipped
// with; on RP2350 the Vult code is regenerated as float (CMake picks the
// right -real flag automatically), on RP2040 it stays Q16 fixed-point.

#include "picoadk/picoadk.h"
#include "vult.h"

#include <FreeRTOS.h>
#include <task.h>

using namespace picoadk;

namespace {

Dsp_process_type g_ctx;

void note_on(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (velocity == 0) { Dsp_noteOff(g_ctx, note, channel); }
    else               { Dsp_noteOn (g_ctx, note, velocity, channel); }
    Controls::set_note_led(velocity > 0);
}

void note_off(uint8_t channel, uint8_t note, uint8_t /*velocity*/) {
    Dsp_noteOff(g_ctx, note, channel);
    Controls::set_note_led(false);
}

void cc(uint8_t channel, uint8_t controller, uint8_t value) {
    Dsp_controlChange(g_ctx, controller, value, channel);
}

void audio_cb(const float* const* /*in*/,
              float* const* out,
              std::size_t frames,
              void* /*user*/) {
    // CV inputs feed the Vult `process(cv0..cv3)` signature.
    const auto cv0 = Controls::read_adc8(0);
    const auto cv1 = Controls::read_adc8(1);
    const auto cv2 = Controls::read_adc8(2);
    const auto cv3 = Controls::read_adc8(3);

    float* L = out[0];
    float* R = out[1];
    for (std::size_t i = 0; i < frames; ++i) {
        Dsp_process(g_ctx, cv0, cv1, cv2, cv3);
        L[i] = static_cast<float>(Dsp_process_ret_0(g_ctx)) * (1.0f / 65536.0f);
        R[i] = static_cast<float>(Dsp_process_ret_1(g_ctx)) * (1.0f / 65536.0f);
    }
}

[[noreturn]] void midi_task(void*) {
    for (;;) {
        Midi::process();
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

[[noreturn]] void watchdog_task(void*) {
    for (;;) {
        System::feed_watchdog();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

}  // namespace

int main() {
    HardwareConfig hw;
    hw.audio.sample_rate_hz = 48000;
    hw.audio.block_size     = 32;
    hw.audio.direction      = AudioDirection::Out;
    hw.midi.inputs          = MidiTransport::Usb | MidiTransport::Uart;
    hw.midi.outputs         = MidiTransport::Usb;
    Hardware::init(hw);

    Dsp_process_init(g_ctx);
    Dsp_default_init(g_ctx);
    Dsp_default(g_ctx);

    Midi::set_note_on (note_on);
    Midi::set_note_off(note_off);
    Midi::set_cc      (cc);

    Audio::set_callback(audio_cb);
    Audio::start();

    xTaskCreate(midi_task,     "MIDI", 1024, nullptr, configMAX_PRIORITIES - 1, nullptr);
    xTaskCreate(watchdog_task, "WDOG", 256,  nullptr, tskIDLE_PRIORITY + 1,     nullptr);

    vTaskStartScheduler();
    for (;;) { __asm volatile("wfi"); }
}
