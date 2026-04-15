// =============================================================================
//  PicoADK v3 — SoundFont 2 / SFZ Player
// -----------------------------------------------------------------------------
//  Drop a `default.sf2` (or `default.sfz` directory) onto the SD card and the
//  board enumerates as a USB-MIDI device — anything you play through it
//  comes out the I2S DAC at full polyphony. v2 (RP2350 + 8 MB PSRAM) is the
//  intended target; the SF2 file lives in PSRAM so the engine has random
//  access without re-reading from SD.
//
//  Build with:
//      -DPICOADK_BOARD=v2_rp2350
//      -DPICOADK_ENABLE_SDFAT=ON
//      -DPICOADK_ENABLE_SDIO=ON          (optional, for full-speed SD)
// =============================================================================

#include "picoadk/picoadk.h"

#include <FreeRTOS.h>
#include <task.h>
#include <cstdio>

using namespace picoadk;
using namespace picoadk::dsp;

namespace {

constexpr float kSampleRate = 48000.0f;

Sf2Player g_sf2;
SfzPlayer g_sfz;
bool      g_use_sfz = false;

void on_note_on(uint8_t channel, uint8_t note, uint8_t vel) {
    if (g_use_sfz) g_sfz.note_on (channel, note, vel);
    else           g_sf2.note_on (channel, note, vel / 127.0f);
}
void on_note_off(uint8_t channel, uint8_t note, uint8_t /*vel*/) {
    if (g_use_sfz) g_sfz.note_off(channel, note);
    else           g_sf2.note_off(channel, note);
}
void on_cc(uint8_t channel, uint8_t cc, uint8_t value) {
    if (!g_use_sfz) g_sf2.cc(channel, cc, value);
}

void audio_cb(const float* const* /*in*/, float* const* out, std::size_t frames, void*) {
    for (std::size_t i = 0; i < frames; ++i) { out[0][i] = 0.0f; out[1][i] = 0.0f; }
    if (g_use_sfz) g_sfz.process(out[0], out[1], frames);
    else           g_sf2.process(out[0], out[1], frames);
}

[[noreturn]] void midi_task(void*)     { for (;;) { Midi::process();         vTaskDelay(pdMS_TO_TICKS(1));   } }
[[noreturn]] void watchdog_task(void*) { for (;;) { System::feed_watchdog(); vTaskDelay(pdMS_TO_TICKS(500)); } }

}  // namespace

int main() {
    HardwareConfig hw;
    hw.audio.sample_rate_hz = (uint32_t)kSampleRate;
    hw.audio.block_size     = 64;
    hw.audio.direction      = AudioDirection::Out;
    hw.midi.inputs          = MidiTransport::Usb | MidiTransport::Uart;
    hw.init_storage         = true;
    Hardware::init(hw);

    if (!Storage::is_mounted()) {
        printf("sf2_player: no SD card; nothing to load.\n");
    } else if (g_sfz.load_from_sd("/default.sfz", /*voices=*/12)) {
        printf("sf2_player: loaded SFZ (%u zones)\n", (unsigned)g_sfz.zone_count());
        g_use_sfz = true;
    } else if (g_sf2.load_from_sd("/default.sf2")) {
        printf("sf2_player: loaded SF2 (%d presets)\n", g_sf2.preset_count());
        g_sf2.set_sample_rate(kSampleRate);
        g_sf2.set_preset(0, 0);
        g_sf2.set_gain(0.5f);
    } else {
        printf("sf2_player: no /default.sf2 or /default.sfz on the card.\n");
    }

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
