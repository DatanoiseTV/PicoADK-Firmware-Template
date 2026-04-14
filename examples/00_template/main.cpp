// =============================================================================
//  PicoADK v3 — Application Scaffold
// -----------------------------------------------------------------------------
//  Copy this directory, rename it, and you have a buildable PicoADK firmware:
//
//      cp -r examples/00_template myapp/
//      cmake -B build/v2 -DPICOADK_BOARD=v2_rp2350 -DPICOADK_APP=myapp
//      cmake --build build/v2
//
//  This template wires up:
//    * One audio callback (you fill in the `// >>> YOUR DSP HERE <<<` block).
//    * MIDI from USB + UART, dispatched into your own functions.
//    * Hardware controls (pots / encoders / buttons) you can read from the
//      callback or background tasks.
//    * A FreeRTOS scheduler so it's safe to do non-trivial work in tasks.
//
//  The HAL surface is `<picoadk/picoadk.h>` — see `include/picoadk/hal/*` for
//  every subsystem. The README has the API tour.
// =============================================================================

#include "picoadk/picoadk.h"

#include <FreeRTOS.h>
#include <task.h>

#include <cmath>

using namespace picoadk;

// ---- Synth state -----------------------------------------------------------
namespace {

constexpr float kSampleRate = 48000.0f;

struct VoiceState {
    bool  gate    = false;
    float freq_hz = 440.0f;
    float phase   = 0.0f;
    float vel     = 0.0f;
} g_voice;

// Hand-rolled DSP module — replace with anything (Vult, libpd, your own,
// the picoadk::dsp module library, …).
inline float synth_tick() {
    if (!g_voice.gate) return 0.0f;
    float v = std::sin(2.0f * 3.14159265f * g_voice.phase);
    g_voice.phase += g_voice.freq_hz / kSampleRate;
    if (g_voice.phase >= 1.0f) g_voice.phase -= 1.0f;
    return v * g_voice.vel * 0.4f;
}

// ---- Audio callback --------------------------------------------------------
void audio_cb(const float* const* /*in*/, float* const* out, std::size_t frames, void*) {
    float* L = out[0];
    float* R = out[1];

    // Read live control values once per block (cheap).
    float pitch_cv = Controls::read_adc8_norm(0);
    g_voice.freq_hz = 110.0f * std::pow(2.0f, pitch_cv * 4.0f);

    for (std::size_t i = 0; i < frames; ++i) {
        // >>> YOUR DSP HERE <<<
        float s = synth_tick();
        L[i] = s;
        R[i] = s;
    }
}

// ---- MIDI callbacks --------------------------------------------------------
void on_note_on(uint8_t /*ch*/, uint8_t note, uint8_t velocity) {
    if (velocity == 0) { g_voice.gate = false; Controls::set_note_led(false); return; }
    g_voice.freq_hz = 440.0f * std::pow(2.0f, (note - 69) / 12.0f);
    g_voice.vel     = velocity / 127.0f;
    g_voice.gate    = true;
    Controls::set_note_led(true);
}
void on_note_off(uint8_t /*ch*/, uint8_t /*note*/, uint8_t /*v*/) {
    g_voice.gate = false;
    Controls::set_note_led(false);
}
void on_cc(uint8_t /*ch*/, uint8_t cc, uint8_t value) {
    // Map CCs to your synth parameters here.
    (void)cc; (void)value;
}

// ---- Background tasks ------------------------------------------------------
[[noreturn]] void midi_task(void*)     { for (;;) { Midi::process();         vTaskDelay(pdMS_TO_TICKS(1));   } }
[[noreturn]] void watchdog_task(void*) { for (;;) { System::feed_watchdog(); vTaskDelay(pdMS_TO_TICKS(500)); } }

}  // namespace

int main() {
    // ---- Hardware bring-up -----------------------------------------------
    HardwareConfig hw;
    hw.audio.sample_rate_hz = (uint32_t)kSampleRate;
    hw.audio.block_size     = 32;
    hw.audio.direction      = AudioDirection::Out;     // change to Duplex for FX

    hw.midi.inputs  = MidiTransport::Usb | MidiTransport::Uart;
    hw.midi.outputs = MidiTransport::Usb;

    // USB device — show up as a CDC serial debug + USB-MIDI by default.
    UsbConfig usb;
    usb.classes = UsbClass::Cdc | UsbClass::Midi;
    usb.product = "PicoADK Template";
    Usb::configure(usb);

    Hardware::init(hw);

    // ---- App wiring -------------------------------------------------------
    Midi::set_note_on (on_note_on);
    Midi::set_note_off(on_note_off);
    Midi::set_cc      (on_cc);

    Audio::set_callback(audio_cb);
    Audio::start();

    xTaskCreate(midi_task,     "MIDI", 1024, nullptr, configMAX_PRIORITIES - 1, nullptr);
    xTaskCreate(watchdog_task, "WDOG", 256,  nullptr, tskIDLE_PRIORITY + 1,     nullptr);

    vTaskStartScheduler();
    for (;;) __asm volatile("wfi");
}
