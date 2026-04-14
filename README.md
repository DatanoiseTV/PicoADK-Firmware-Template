# PicoADK Firmware Template — v3

Cross-board firmware template for the **PicoADK v1 (RP2040)** and **PicoADK v2 (RP2350)** audio dev kits. Provides a clean, modern HAL on top of the Pico SDK so the same application code can run on both boards: same audio callback signature, same MIDI dispatch, same control surface, same DSP types — the differences (FPU vs fixed-point, PSRAM, SDIO, M33 DSP intrinsics) are handled inside the HAL.

> **Status:** v3 is an active rewrite on the `v3-refactor` branch. The structure and public API land first; PIO engines, PSRAM, SDIO, the DSP module library and the libpd/sampler engines follow in subsequent phases. The legacy v1/v2 firmware remains intact on `main` and on the `RP235x*` branches.

## Hardware

* **PicoADK v1** — RP2040 dual-core Cortex-M0+, I²S DAC, MCP3208 ADC, USB-MIDI, 5-pin DIN MIDI.
* **PicoADK v2** — RP2350A dual-core Cortex-M33 with single-precision FPU and DSP extensions, 4 MB flash, **8 MB QSPI PSRAM**, microSD with **SDIO 4-bit** support, USB-C, optocoupled MIDI in.

The way-to-go hardware option is the [PicoADK](https://github.com/DatanoiseTV/PicoADK-Hardware).

## Layout

```
boards/                 board-specific pin maps + feature flags (v1, v2)
cmake/picoadk/          CMake helpers (board, FreeRTOS, Vult)
include/picoadk/        public HAL headers
  hal/                    audio, midi, controls, storage, psram, display,
                          gfx, encoders, buttons, mux, usb_device, hardware
  dsp/                    real (q16/float), math, modules, sample_player, pd
src/
  hal/                    HAL implementations
  midi/                   MIDI transports (USB, UART, dispatcher)
  usb/                    TinyUSB descriptors / serial number
  dsp/                    DSP helpers
  third_party/            vendored libraries (TLSF, libpd, SdFat, …)
examples/
  02_synth_vult/          the original Vult monosynth, ported to v3
vultsrc/                  Vult DSP source (unchanged)
lib/                      git submodules: pico-extras, vult, FreeRTOS-Kernel, …
```

## Building

```
git submodule update --init --recursive

# v1 (RP2040)
cmake -B build/v1 -DPICOADK_BOARD=v1_rp2040
cmake --build build/v1

# v2 (RP2350)
cmake -B build/v2 -DPICOADK_BOARD=v2_rp2350
cmake --build build/v2
```

Pick a different example with `-DPICOADK_APP=examples/<name>`. Override the Vult numeric type with `-DPICOADK_VULT_REAL=float|fixed` and the engine sample rate with `-DPICOADK_VULT_SAMPLERATE=48000`.

## Designed-in capabilities

* **Dual board** from one source tree. CMake selects the platform, FreeRTOS port, default clocks, Vult numeric type (Q16 on v1, float on v2), and PSRAM/SDIO support.
* **Audio** — runtime-configurable sample rate (8 k–192 k), bit depth (16/24/32), block size, and direction (out / in / full duplex). Single callback signature whether you take floats or raw int32.
* **MIDI** — USB and UART (5-pin DIN) simultaneously. One callback set, transport reported per message.
* **USB composite** — CDC serial, USB-MIDI (multi-cable), and USB MSC exposing internal flash storage, the SD card, or both as separate LUNs.
* **Controls** — MCP3208 SPI ADC, internal SAR ADC, multiplexed banks (CD4051 / 74HC4067), debounced buttons, fast PIO-driven encoder banks.
* **Display** — SSD1306 / SH1106 / SSD1309 panels with a shared `gfx::Canvas` (lines, rects, circles, text, blits, double-buffered DMA present).
* **Storage** — SdFat with FAT32 / exFAT and long file names. SDIO 4-bit on v2, SPI fallback on v1.
* **PSRAM** — TLSF allocator on v2 (~8 MB usable), `PICOADK_PSRAM_BSS` / placement-`new (PSRAM)` for static and dynamic placement. No-op fallbacks on v1 so code compiles either way.
* **DSP toolkit** — `picoadk::dsp::Real` adapts between Q16 and float; hardware-accelerated math helpers (RP2040 hw divider + interp, M33 SIMD intrinsics, DCP for double precision); a module library covering oscillators, filters, envelopes, dynamics, delay, reverb.
* **Sampler** — streaming or RAM-resident sample players, multisample key-mapping, pitch shift, loop, optional time-stretch.
* **PureData** — headless libpd embedded; load patches from flash buffer or SD; route through the audio callback.
* **VoiceLink** — wire several boards together over UART for distributed polyphony / hybrid synths / "voice cards". DMA-fed framing keeps the audio thread untouched.
* **USB host** — plug in a Launchpad / MIDI keyboard / controller. Native single-port-role or PIO-second-port (`pico_pio_usb`) for simultaneous device + host.
* **MIDI utilities** — RPN / NRPN aggregation, 14-bit pitch bend, channel-mode messages, MIDI clock; Launchpad programmer-mode driver with grid coordinates and RGB LED helpers.

## Application skeleton

```cpp
#include "picoadk/picoadk.h"
using namespace picoadk;

void audio_cb(const float* const*, float* const* out, std::size_t frames, void*) {
    for (std::size_t i = 0; i < frames; ++i) {
        out[0][i] = out[1][i] = 0.0f;
    }
}

int main() {
    HardwareConfig hw;
    hw.audio.sample_rate_hz = 48000;
    hw.audio.direction      = AudioDirection::Duplex;
    hw.midi.inputs          = MidiTransport::Usb | MidiTransport::Uart;
    Hardware::init(hw);

    Audio::set_callback(audio_cb);
    Audio::start();

    while (true) {
        Midi::process();
        System::feed_watchdog();
    }
}
```

## Migration

The legacy `src/main.cpp` / `src/picoadk_hw.cpp` / `lib/audio` paths remain on `main` and on tags. The `v3-refactor` branch supersedes them with the layout above; `examples/02_synth_vult/main.cpp` reproduces the old Vult monosynth behaviour against the new API.

## Community

PicoADK on Discord: [discord.gg/BsHUEdStMt](https://discord.gg/BsHUEdStMt). Discussions: [PicoADK-Hardware](https://github.com/DatanoiseTV/PicoADK-Hardware/discussions).
