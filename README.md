# PicoADK Firmware Template — v3

> **A clean-slate rewrite.** v3 is a from-the-ground-up redesign of the PicoADK firmware template — same hardware, brand new HAL. One source tree builds for **PicoADK v1 (RP2040)** and **PicoADK v2 (RP2350)**. Same audio callback. Same MIDI dispatch. Same DSP types. The differences (Cortex-M33 FPU, PSRAM, SDIO, M33 DSP intrinsics) are handled inside the HAL so application code never has to ifdef.

```
                                                          ┌────────────────────┐
                       ┌── audio cb ──→ DAC / I2S ────────│ runtime SR/bit/    │
                       │                                  │ TDM out / in /     │
                       │                                  │ duplex             │
   USB-MIDI ──┐        │                                  └────────────────────┘
   UART-MIDI ─┼─→ Midi ┤        ┌─→ pots / mux / scanner
   USB host ──┘        │        │
                       │   Hardware                       ┌────────────────────┐
                       ├─→ Controls ── encoders / buttons │ smart updates,     │
                       │                                  │ debounced, fast    │
                       ├─→ Display ── u8g2 (SSD1306 …)    └────────────────────┘
                       ├─→ Storage ── SdFat (FAT32/exFAT/LFN)
                       ├─→ PSRAM ── TLSF allocator
                       └─→ VoiceLink ── multi-board polyphony over UART
```

## What's new in v3

* **One source tree, two boards.** `cmake -DPICOADK_BOARD=v1_rp2040` or `v2_rp2350`. The right pico-sdk port, FreeRTOS port, default clocks, Vult numeric type, and PSRAM/SDIO support are picked automatically.
* **Audio engine that adapts at runtime.** Sample rate, bit depth (16/24/32), block size, and direction (output / input / **full duplex**) are `AudioConfig` fields, not compile-time decisions. **TDM4 / TDM8** + left/right-justified wire formats so codecs like the PCM3168 / AK4954 / TLV320 work without HAL changes.
* **MIDI everywhere.** USB device, UART (5-pin DIN), and **USB host** (Launchpad / keyboards / controllers via `pico_pio_usb`) — all simultaneously, all with one callback set.
* **General MIDI handler.** RPN/NRPN aggregation, 14-bit pitch bend with configurable range, channel-mode messages, MIDI clock + transport. Drop in a `Handler` struct of function pointers and you're done.
* **Launchpad programmer-mode driver.** Grid-coordinate API for pads + buttons, RGB LEDs, palette colours. Diff-only updates batch into one sysex frame when ≥8 pads change — never resending the whole grid.
* **USB composite device** picked at runtime: CDC (serial debug) + USB-MIDI (multi-cable) + MSC (mass-storage exposing internal flash, the SD card, or both as separate LUNs).
* **Display via u8g2.** SSD1306 / SH1106 / SSD1309 with the full u8g2 font catalog and our `gfx::Canvas` sugar wrapping the common calls. Optional LVGL backend for TFT projects.
* **Controls toolkit.** External MCP3208 ADC, internal SAR ADC, mux scanner (CD4051 / 74HC4067 chains, mixed with direct inputs), polled quadrature encoders (and a PIO-bank version coming), 1 kHz debounced buttons with hold + double-click.
* **PSRAM with TLSF.** v2's 8 MB QSPI PSRAM is split into linker-reserved sections (`PICOADK_PSRAM_BSS` / `PICOADK_PSRAM_DATA`) plus a TLSF heap (`Psram::alloc`, placement-`new (PSRAM)`). v1 stubs out to the system heap so code compiles either way.
* **SDIO 4-bit on v2** via the proven [`rabbitholecomputing/SDIO_RP2350`](https://github.com/rabbitholecomputing/SDIO_RP2350) driver, surfaced through SdFat with FAT32 / exFAT / long file name support.
* **DSP that scales with the silicon.** `picoadk::dsp::Real` is `float` on v2 (M33 FPU) and Q16 fixed-point on v1. Hardware helpers wrap RP2040's hw divider + interp blocks and the RP2350's M33 SIMD + DCP coprocessor. CMSIS-DSP integration optional.
* **DSP module library.** BLEP / wavetable / noise oscillators, ADSR, state-variable + ladder filters, saturator, wavefolder, delay, FDN reverb, compressor.
* **Sampler module.** Stream samples from SD with prefetch ring, pitch-shift, optional time-stretch, looping, multisample key-mapping for "play any sample on any keyboard zone".
* **Headless PureData.** Embed `libpd`, load patches from a memory buffer or SD, route through the audio callback. Optional via `-DPICOADK_LIBPD=ON`.
* **VoiceLink.** Wire multiple PicoADKs together over a 3 Mbaud UART for distributed polyphony and "voice cards". Each board reports its own polyphony capacity; the master allocator (round-robin / oldest / quietest) routes notes accordingly. DMA-fed framing keeps the audio thread untouched.
* **Pinned, supported deps.** `pico-sdk 2.2.0`, `pico-extras sdk-2.2.0`, the upstream `FreeRTOS-Kernel V11.3.0` (RPi-supported `ThirdParty/GCC/RP2040` SMP port for v1, `GCC/ARM_CM33_NTZ/non_secure` for v2), `pico-pio-usb`, `u8g2`. CMakePresets.json + `.vscode/` configured for the official Raspberry Pi Pico VS Code extension.

## Hardware

| | PicoADK v1 | PicoADK v2 |
|---|---|---|
| MCU       | RP2040 (dual M0+) | RP2350A (dual M33 + FPU + DCP) |
| Flash     | 4 MB QSPI         | 4 MB QSPI                       |
| PSRAM     | —                 | 8 MB QSPI (TLSF heap)           |
| SD        | SPI               | **SDIO 4-bit (high speed)**     |
| MIDI      | USB + DIN         | USB + DIN + USB host (PIO USB)  |
| ADC       | MCP3208 (SPI), 4×SAR | same                         |
| Display   | SSD1306 I²C       | SSD1306 / TFT SPI               |

## Build

```sh
git clone --recursive https://github.com/DatanoiseTV/PicoADK-Firmware-Template
cd PicoADK-Firmware-Template

# PicoADK v1
cmake --preset v1
cmake --build build/v1

# PicoADK v2
cmake --preset v2
cmake --build build/v2
```

Pick a different example with `-DPICOADK_APP=examples/<name>`:

| App | Description |
|---|---|
| `examples/00_template`   | Scaffold to copy when starting a new project. |
| `examples/01_passthrough`| Smallest possible app — full-duplex I2S passthrough. |
| `examples/02_synth_vult` | Original PicoADK Vult monosynth, ported to v3 (default). |

Override at configure time:
* `-DPICOADK_VULT_REAL=float\|fixed` — Vult numeric type (auto-picks per board).
* `-DPICOADK_VULT_SAMPLERATE=48000` — sample rate baked into Vult tables.
* `-DPICOADK_ENABLE_SDFAT=ON` — enable SdFat backend (Phase 4).
* `-DPICOADK_ENABLE_SDIO=ON` — pull in the RP2350 SDIO driver (v2 only).
* `-DPICOADK_LIBPD=ON` — embed libpd.

## Application skeleton

```cpp
#include "picoadk/picoadk.h"
#include <FreeRTOS.h>
#include <task.h>

using namespace picoadk;

void audio_cb(const float* const* in, float* const* out, std::size_t frames, void*) {
    for (std::size_t i = 0; i < frames; ++i) {
        out[0][i] = out[1][i] = 0.0f;        // your DSP here
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
    vTaskStartScheduler();
    for (;;) __asm volatile("wfi");
}
```

## Layout

```
boards/                   board pin maps + feature flags + pico-sdk board headers
cmake/picoadk/            board.cmake, freertos.cmake, vult.cmake, sdio.cmake,
                          sdfat.cmake, libpd.cmake
include/picoadk/          public HAL — one header per subsystem
  hal/                       audio, midi, controls, storage, psram, gfx,
                             display, encoders, buttons, mux, usb_device,
                             usb_host, voicelink, hardware
  midi/                      general_handler (NRPN/RPN/PB), launchpad
  dsp/                       real (Q16/float), math, modules, sample_player, pd
src/                      implementations (PIO, DMA, IRQ, conversion)
  hal/pio/                   PIO programs (i2s_stereo, tdm, sdio_4bit, …)
  third_party/               vendored libraries (TLSF, …)
examples/                 ready-to-build apps
lib/                      submodules: pico-sdk 2.2.0, pico-extras sdk-2.2.0,
                          FreeRTOS-Kernel V11.3.0, pico-pio-usb, u8g2, vult
```

## Migration from v1/v2 firmware

The legacy `src/main.cpp` / `picoadk_hw` / `lib/audio` / `lib/oled` paths remain on `main` and on the `RP235x*` branches. The v3 layout supersedes them with the table above; `examples/02_synth_vult/main.cpp` reproduces the original Vult monosynth behaviour against the new HAL. None of the legacy code is wired into the v3 build — the rewrite is total.

## Status & roadmap

Phase 0 (foundation, structure, public API) and Phase 1 (audio engine + math + PSRAM + controls + USB composite + USB host + MIDI handlers + VoiceLink) are implemented. Hardware bring-up (PIO timing, SDIO, USB host enumeration, PSRAM init) is the obvious next step and needs real boards on the bench. CI matrix building both targets and a docs site round out Phase 5.

## Community

PicoADK on Discord: [discord.gg/BsHUEdStMt](https://discord.gg/BsHUEdStMt). Discussions: [PicoADK-Hardware](https://github.com/DatanoiseTV/PicoADK-Hardware/discussions).
