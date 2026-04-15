// =============================================================================
//  PicoADK v3 — Multisampler
// -----------------------------------------------------------------------------
//  Loads a key-mapped sample set from the SD card and plays it polyphonically
//  in response to incoming MIDI notes (any transport: USB device, UART, USB
//  host).
//
//  Drop a few WAVs onto the card:
//      /sounds/piano_C2.wav   → covers MIDI 24..47, root 36
//      /sounds/piano_C4.wav   → covers MIDI 48..71, root 60
//      /sounds/piano_C6.wav   → covers MIDI 72..95, root 84
//
//  Build with: -DPICOADK_ENABLE_SDFAT=ON  (and -DPICOADK_ENABLE_SDIO=ON on v2
//  for high-speed access).
// =============================================================================

#include "picoadk/picoadk.h"

#include <FreeRTOS.h>
#include <task.h>
#include <cstdio>

using namespace picoadk;
using namespace picoadk::dsp;

namespace {

constexpr float kSampleRate = 48000.0f;

MultisamplePlayer g_player;

// 3 zones across the keyboard — extend for richer coverage. The sources are
// loaded in main() because they own SD-resident memory.
KeyZone g_zones[3];

void note_on(uint8_t /*ch*/, uint8_t note, uint8_t vel) {
    g_player.note_on(note, vel);
    Controls::set_note_led(true);
}
void note_off(uint8_t /*ch*/, uint8_t note, uint8_t /*vel*/) {
    g_player.note_off(note);
    Controls::set_note_led(false);
}

void audio_cb(const float* const* /*in*/, float* const* out, std::size_t n, void*) {
    Real lbuf[128], rbuf[128];
    if (n > 128) n = 128;
    g_player.process(lbuf, rbuf, n);
    for (std::size_t i = 0; i < n; ++i) {
        out[0][i] = to_float(lbuf[i]);
        out[1][i] = to_float(rbuf[i]);
    }
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
        printf("multisampler: no SD card mounted; nothing to play.\n");
    } else {
        g_zones[0] = { MemorySampleSource::load_wav_from_sd("/sounds/piano_C2.wav"),
                       /*lo*/24, /*hi*/47, /*root*/36, 1.0f };
        g_zones[1] = { MemorySampleSource::load_wav_from_sd("/sounds/piano_C4.wav"),
                       48, 71, 60, 1.0f };
        g_zones[2] = { MemorySampleSource::load_wav_from_sd("/sounds/piano_C6.wav"),
                       72, 95, 84, 1.0f };
        g_player.reset(kSampleRate, 8);
        g_player.set_zones(g_zones, 3);
    }

    Midi::set_note_on(note_on);
    Midi::set_note_off(note_off);

    Audio::set_callback(audio_cb);
    Audio::start();

    xTaskCreate(midi_task,     "MIDI", 1024, nullptr, configMAX_PRIORITIES - 1, nullptr);
    xTaskCreate(watchdog_task, "WDOG", 256,  nullptr, tskIDLE_PRIORITY + 1,     nullptr);
    vTaskStartScheduler();
    for (;;) __asm volatile("wfi");
}
