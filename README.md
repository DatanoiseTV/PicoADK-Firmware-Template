<h1 align="center">PicoADK Firmware Template</h1>

<p align="center"><em>Pro-quality audio firmware. One source tree. Two boards.</em></p>

<p align="center">
  <a href="https://github.com/DatanoiseTV/PicoADK-Firmware-Template/actions"><img alt="build" src="https://img.shields.io/github/actions/workflow/status/DatanoiseTV/PicoADK-Firmware-Template/build.yml?branch=main"></a>
  <img alt="boards" src="https://img.shields.io/badge/boards-RP2040%20%7C%20RP2350-blue">
  <img alt="freertos" src="https://img.shields.io/badge/FreeRTOS-SMP%20V11-success">
  <img alt="license" src="https://img.shields.io/badge/license-BSD--3-lightgrey">
</p>

PicoADK is the audio dev kit for the Raspberry Pi RP2040 (v1) and RP2350 (v2). This template is the firmware starting point: a clean HAL on top of the Pico SDK that hides the silicon differences (FPU, PSRAM, SDIO, M33 DSP intrinsics) so the same audio callback runs on both boards without `#ifdef`s. Drop in your DSP, pick from a tray of ready-made examples, build either board with one command.

## Quickstart

```sh
git clone --recursive https://github.com/DatanoiseTV/PicoADK-Firmware-Template
cd PicoADK-Firmware-Template
cmake --preset v2                        # or v1
cmake --build build/v2
```

The UF2 lands in `build/v2/app/picoadk_app.uf2`. Hold BOOTSEL, plug in the board, drag-drop. You're running.

```cpp
#include "picoadk/picoadk.h"
#include <FreeRTOS.h>
#include <task.h>

using namespace picoadk;

void audio(const float* const* /*in*/, float* const* out, std::size_t n, void*) {
    for (std::size_t i = 0; i < n; ++i)
        out[0][i] = out[1][i] = std::sin(2.0f * 3.14159f * 440.0f * i / 48000.0f);
}

int main() {
    HardwareConfig hw;
    hw.audio.sample_rate_hz = 48000;
    hw.midi.inputs          = MidiTransport::Usb | MidiTransport::Uart;
    Hardware::init(hw);
    Audio::set_callback(audio);
    Audio::start();
    vTaskStartScheduler();
}
```

Boots, plays a 440 Hz sine, accepts MIDI on USB and DIN. That's the whole program.

## What's in the box

**Audio engine** with runtime-configurable sample rate (8 k–192 k), bit depth (16/24/32), block size, direction (out · in · full duplex), and frame format (I²S · left/right justified · TDM4 · TDM8). PIO + DMA, off-IRQ FreeRTOS audio task.

**MIDI** over USB device, UART (5-pin DIN), and USB host (Launchpad / keyboards via `pico_pio_usb`) — all three simultaneously through one callback set. NRPN/RPN aggregation, 14-bit pitch bend, MIDI clock, channel-mode messages.

**USB composite** assembled at runtime — CDC serial debug, USB-MIDI (multi-cable), and MSC exposing internal flash and/or the SD card as separate LUNs. Mix and match.

**DSP toolkit.** `dsp::Real` is `float` on RP2350 (FPU) and Q16 fixed-point on RP2040; same code compiles for both. Module library: BLEP / wavetable / noise oscillators, ADSR, state-variable + ladder filters, saturator, wavefolder, delay (PSRAM-backed), 8-line FDN reverb (PSRAM-backed), compressor.

**Sampler** — memory-resident or SD-streaming WAV (16/24/32-bit), pitch shift, looping, multisample keymap; **SoundFont 2** via TinySoundFont (multisample/loop/release/modulators); embedded **SFZ** subset parser that builds streaming sources per region. Per-source 4096-frame prefetch ring keeps SD seeks at note-on time only.

**PSRAM** — TLSF allocator over the v2's 8 MB QSPI PSRAM. Linker section macros (`PICOADK_PSRAM_BSS`/`_DATA`), `placement-new (PSRAM)`. Falls back to the system heap on v1 so portable code Just Works.

**Storage** — SdFat with FAT32 / exFAT / **long file names**. SDIO 4-bit on v2 ([rabbitholecomputing/SDIO_RP2350](https://github.com/rabbitholecomputing/SDIO_RP2350)), SPI on v1.

**Display** via [u8g2](https://github.com/olikraus/u8g2) — SSD1306 / SH1106 / SSD1309 over I²C or SPI, full font catalog, `gfx::Canvas` C++ wrapper for one-liners.

**Controls toolkit** — MCP3208 SPI ADC, internal SAR ADC, mux banks (CD4051 / 74HC4067), polled quadrature encoders, debounced buttons with hold + double-click.

**VoiceLink** — wire several PicoADKs together over a 3 Mbaud UART for distributed polyphony / voice cards. Auto-enumerated chain, per-card capacity, oldest/quietest/round-robin allocation. DMA-fed framing keeps the audio thread untouched.

**PureData** (optional, `-DPICOADK_LIBPD=ON`) — embed libpd, load patches from flash or SD, route through the audio callback.

**FreeRTOS SMP on both boards** via the Raspberry Pi-curated kernel fork. `xTaskCreatePinnedToCore()` works ESP32-style. Stdio routes to UART by default and USB-CDC when you ask for it.

## Examples

| | What it shows |
|---|---|
| `examples/00_template`        | Copy-and-edit scaffold — audio + MIDI + controls + USB composite already wired. |
| `examples/01_passthrough`     | The minimal program — full-duplex passthrough in 25 lines. |
| `examples/02_synth_vult`      | Original PicoADK Vult monosynth, ported to v3. |
| `examples/03_fx_rack`         | Stereo delay + Hadamard FDN reverb in parallel; pots morph time / feedback / size / mix; PSRAM-backed buffers on v2. |
| `examples/04_multisampler`    | Key-mapped piano loaded from SD, 8-voice polyphony with pitch-shift to root. |
| `examples/05_midi_controller` | No audio — pots / encoder / button forwarded as USB-MIDI CCs. |
| `examples/06_sf2_player`      | SoundFont 2 / SFZ player. Drop a `default.sf2` (or `default.sfz`) on the SD card. |

```sh
cmake --preset v2 -DPICOADK_APP=examples/03_fx_rack
cmake --build build/v2
```

## Build

| Preset | Board | Build type |
|---|---|---|
| `v1`        | RP2040 | Release |
| `v1-debug`  | RP2040 | Debug   |
| `v2`        | RP2350 | Release |
| `v2-debug`  | RP2350 | Debug   |

Useful flags:

| Flag | Default | Effect |
|---|---|---|
| `-DPICOADK_APP=<dir>`              | `examples/02_synth_vult` | App folder to build. |
| `-DPICOADK_VULT_REAL=float\|fixed` | auto                     | Vult numeric type (float on v2, Q16 on v1). |
| `-DPICOADK_VULT_SAMPLERATE=48000`  | `48000`                  | Sample rate baked into Vult tables. |
| `-DPICOADK_ENABLE_SDFAT=ON`        | OFF                      | Pull in SdFat for Storage HAL. |
| `-DPICOADK_ENABLE_SDIO=ON`         | OFF                      | Pull in `SDIO_RP2350` (v2 only). |
| `-DPICOADK_ENABLE_USB_HOST=ON`     | OFF                      | Compile the TinyUSB host stack. |
| `-DPICOADK_LIBPD=ON`               | OFF                      | Embed libpd. |

> **VS Code:** install the official Raspberry Pi Pico extension. `CMakePresets.json` and `.vscode/` are pre-wired — pick a preset from the status bar.

## Hardware

|             | PicoADK v1                | PicoADK v2 |
|---          |---                        |---|
| **MCU**     | RP2040 — dual M0+         | RP2350A — dual M33 + FPU + DCP |
| **Flash**   | 4 MB QSPI                 | 4 MB QSPI |
| **PSRAM**   | —                         | **8 MB QSPI** |
| **SD**      | SPI                       | **SDIO 4-bit** |
| **MIDI in** | USB + DIN                 | USB + DIN + USB host (PIO USB) |
| **Display** | SSD1306 I²C               | SSD1306 / TFT SPI |

[PicoADK Hardware repo](https://github.com/DatanoiseTV/PicoADK-Hardware)

## Documentation

| | |
|---|---|
| [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) | Layered model, audio pipeline diagram, threading model, memory map, file-by-file map. |
| [`docs/HAL.md`](docs/HAL.md)                   | Per-subsystem reference with code samples for every public API. |
| [`CHANGELOG.md`](CHANGELOG.md)                 | What changed in v3.0.0. |
| [`CONTRIBUTING.md`](CONTRIBUTING.md)           | House rules + how to add a HAL subsystem / DSP module. |

## Pinned dependencies

| | |
|---|---|
| pico-sdk         | `2.2.0`     |
| pico-extras      | `sdk-2.2.0` |
| FreeRTOS-Kernel  | `raspberrypi/FreeRTOS-Kernel` (SMP for both boards) |
| pico-pio-usb     | latest      |
| u8g2             | latest      |
| TinySoundFont    | latest      |
| SDIO_RP2350 / SdFat / libpd | fetched on demand via CMake when the matching `-DPICOADK_ENABLE_…` flag is on |

## Status

All six example apps build and link clean for both boards (verified with `arm-none-eabi-gcc 14.2`); CI builds both targets on every push and runs the host-side DSP unit tests (`tests/`, **10/10 pass**). PIO timing for the new I²S/TDM/SDIO programs and PSRAM init on v2 still need real boards on the bench — the structure and call sites are in place; values that need verification are clearly marked in the source.

The legacy firmware is preserved on the `RP235x*` branches and on tags. Nothing was removed from there.

---

<p align="center"><sub>BSD-3-Clause · for people who like their synths to sound exactly like they wanted</sub></p>
