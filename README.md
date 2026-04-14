<h1 align="center">PicoADK Firmware Template</h1>

<p align="center">
  <a href="https://github.com/DatanoiseTV/PicoADK-Firmware-Template/actions"><img alt="build" src="https://img.shields.io/github/actions/workflow/status/DatanoiseTV/PicoADK-Firmware-Template/build.yml?branch=v3-refactor"></a>
  <img alt="boards" src="https://img.shields.io/badge/boards-v1%20RP2040%20%7C%20v2%20RP2350-blue">
  <img alt="freertos" src="https://img.shields.io/badge/FreeRTOS-SMP%20V11-success">
  <img alt="license" src="https://img.shields.io/badge/license-BSD--3-lightgrey">
</p>

> A clean-slate v3 of the PicoADK firmware. Same audio callback for both boards. Same MIDI dispatch. Same DSP types. The differences (Cortex-M33 FPU, PSRAM, SDIO, M33 DSP intrinsics) live inside the HAL — your code never has to ifdef.

```sh
git clone --recursive https://github.com/DatanoiseTV/PicoADK-Firmware-Template
cmake --preset v2 && cmake --build build/v2
```

Output: `build/v2/app/picoadk_app.uf2`. Drag-and-drop to the BOOTSEL drive and you're running.

---

## Hello, world

```cpp
#include "picoadk/picoadk.h"
#include <FreeRTOS.h>
#include <task.h>

using namespace picoadk;

void audio(const float* const* in, float* const* out, std::size_t n, void*) {
    for (std::size_t i = 0; i < n; ++i) {
        out[0][i] = out[1][i] = std::sin(2.0f * 3.14159f * 440.0f * i / 48000.0f);
    }
}

int main() {
    HardwareConfig hw;
    hw.audio.sample_rate_hz = 48000;
    hw.audio.direction      = AudioDirection::Out;
    hw.midi.inputs          = MidiTransport::Usb | MidiTransport::Uart;
    Hardware::init(hw);

    Audio::set_callback(audio);
    Audio::start();
    vTaskStartScheduler();
}
```

That's the whole program. Boots, plays a sine wave, accepts MIDI on USB and DIN.

---

## What you get

### Audio you can configure at runtime
```cpp
hw.audio.sample_rate_hz = 96000;        // 8 k – 192 k
hw.audio.bit_depth      = AudioBitDepth::Bits24;
hw.audio.direction      = AudioDirection::Duplex;
hw.audio.frame_format   = AudioFrameFormat::TDM;
hw.audio.slots_per_frame = 8;           // TDM4 / TDM8 for codecs like PCM3168
```
PIO + DMA engine. Off-IRQ FreeRTOS audio task. Nothing hardcoded.

### MIDI everywhere, one callback
```cpp
hw.midi.inputs = MidiTransport::Usb | MidiTransport::Uart | MidiTransport::UsbHost;
Midi::set_note_on([](uint8_t ch, uint8_t note, uint8_t vel) { /* … */ });
```
USB device, UART (DIN), USB host (Launchpad / keyboards) — all simultaneous, all routed through the same callbacks. NRPN/RPN aggregation, 14-bit pitch bend, MIDI clock, channel mode messages — handled.

### USB built at runtime
```cpp
UsbConfig usb;
usb.classes      = UsbClass::Cdc | UsbClass::Midi | UsbClass::Msc;
usb.msc_backend  = UsbMscBackend::SdCard;     // or FlashFs, or Both
Usb::configure(usb);
```
CDC for serial debug. USB-MIDI multi-cable. MSC exposing internal flash and/or the SD card as separate LUNs.

### DSP that scales with the silicon
```cpp
using namespace picoadk::dsp;
LadderFilter filt;         filt.reset(48000.0f);
DelayLine    delay;        delay.configure_auto(2.0f, 48000.0f);   // PSRAM if available
FdnReverb    verb;         verb.configure(3.5f, 48000.0f);          // PSRAM if available
```
`Real` is `float` on RP2350 (FPU) and Q16 fixed-point on RP2040. Hardware-accelerated math (RP2040 hw divider + interp; M33 SIMD + DCP). FX with serious memory hunger live in PSRAM automatically.

### Controls toolkit
- MCP3208 SPI ADC (8 channels, 12-bit) and the internal SAR ADC.
- Multiplexer banks (CD4051 / 74HC4067) intermixed with direct inputs in one scan table.
- Polled quadrature encoders with debounced buttons.
- 1 kHz timer-driven button scan with hold + double-click events.

### Display via [u8g2](https://github.com/olikraus/u8g2)
SSD1306 / SH1106 / SSD1309 over I²C or SPI. The full u8g2 font catalog. `gfx::Canvas` C++ wrapper for one-liners. (Optional LVGL backend planned for TFT projects.)

### Storage that knows long filenames
[SdFat](https://github.com/greiman/SdFat) with FAT32 / exFAT / **LFN**. SDIO 4-bit on v2 ([rabbitholecomputing/SDIO_RP2350](https://github.com/rabbitholecomputing/SDIO_RP2350)), SPI on v1.

### PSRAM with a real allocator
```cpp
auto* big_buffer  = (float*)Psram::alloc(2 * 1024 * 1024);
PICOADK_PSRAM_BSS float wavetable[1 << 16];
SamplePlayer p; p.set_source(MemorySampleSource::load_wav_from_sd("/grand.wav"));
```
TLSF heap over the v2's 8 MB QSPI PSRAM. Linker section macros (`PICOADK_PSRAM_BSS`, `PICOADK_PSRAM_DATA`). Placement-`new (PSRAM)`. Stubbed to system heap on v1 so code stays portable.

### PureData
Optional [libpd](https://github.com/libpd/libpd) embedded — load patches from flash or SD, route them through the audio callback. Enable with `-DPICOADK_LIBPD=ON`.

### VoiceLink — multi-board polyphony
Wire several PicoADKs over a 3 Mbaud UART for "voice cards". Auto-enumerated chain. Each board declares its polyphony; the master allocates voices using round-robin, oldest, or quietest policies. DMA-fed framing keeps the audio thread untouched.

### FreeRTOS SMP, both boards
Raspberry Pi's curated kernel fork. `xTaskCreatePinnedToCore()` works ESP32-style. Stdio routes to UART by default and USB-CDC when you ask for it.

---

## Examples

| Folder | What it shows |
|---|---|
| `examples/00_template`    | Copy-and-edit scaffold with audio + MIDI + controls + USB composite already wired. |
| `examples/01_passthrough` | Minimal full-duplex passthrough. ~25 lines. |
| `examples/02_synth_vult`  | The original PicoADK Vult monosynth, ported to v3. |

```sh
cmake --preset v2 -DPICOADK_APP=examples/00_template
cmake --build build/v2
```

---

## Build

```sh
cmake --preset v1                # PicoADK v1 (RP2040)  Release
cmake --preset v1-debug          # ... Debug
cmake --preset v2                # PicoADK v2 (RP2350)  Release
cmake --preset v2-debug          # ... Debug
```

| Flag | What it does |
|---|---|
| `-DPICOADK_APP=<dir>`              | App folder (default: `examples/02_synth_vult`). |
| `-DPICOADK_VULT_REAL=float\|fixed` | Vult numeric type (auto: float on v2, Q16 on v1). |
| `-DPICOADK_VULT_SAMPLERATE=48000`  | SR baked into Vult tables. |
| `-DPICOADK_ENABLE_SDIO=ON`         | Pull in `SDIO_RP2350` (v2). |
| `-DPICOADK_ENABLE_SDFAT=ON`        | Pull in SdFat. |
| `-DPICOADK_LIBPD=ON`               | Embed libpd. |

> **VS Code:** install the official **Raspberry Pi Pico** extension. `CMakePresets.json` and `.vscode/` are pre-wired — pick a preset from the status bar and you're building.

---

## Hardware

|  | PicoADK v1 | PicoADK v2 |
|---|---|---|
| **MCU**     | RP2040 — dual M0+ | RP2350A — dual M33 + FPU + DCP |
| **Flash**   | 4 MB QSPI         | 4 MB QSPI                       |
| **PSRAM**   | —                 | **8 MB QSPI**                   |
| **SD**      | SPI               | **SDIO 4-bit**                  |
| **MIDI in** | USB + DIN         | USB + DIN + USB host (PIO USB)  |
| **ADC**     | MCP3208 (SPI)     | MCP3208 (SPI)                   |
| **Display** | SSD1306 I²C       | SSD1306 / TFT SPI               |

[PicoADK Hardware repo](https://github.com/DatanoiseTV/PicoADK-Hardware)

---

## Layout

```
boards/                board pin maps + pico-sdk board headers + linker fragments
cmake/picoadk/         board / freertos / vult / sdio / sdfat / libpd helpers
include/picoadk/       public HAL — one header per subsystem
  hal/                   audio · midi · controls · storage · psram · gfx · display
                         encoders · buttons · mux · usb_device · usb_host
                         voicelink · hardware
  midi/                  general_handler (NRPN/RPN/PB) · launchpad
  dsp/                   real (Q16/float) · math · modules · sample_player · pd
src/                   implementations (PIO · DMA · IRQ · conversions)
  hal/pio/                 PIO programs
  third_party/             vendored libraries (TLSF, …)
examples/              ready-to-build apps
lib/                   submodules (pinned)
```

---

## Pinned dependencies

| | |
|---|---|
| pico-sdk         | `2.2.0`     |
| pico-extras      | `sdk-2.2.0` |
| FreeRTOS-Kernel  | `raspberrypi/FreeRTOS-Kernel` (SMP for both boards) |
| pico-pio-usb     | latest      |
| u8g2             | latest      |

---

## Status

v3 is a clean-slate rewrite on the `v3-refactor` branch. **Both boards build and link with the v3 HAL today.** Hardware bring-up of the new PIO programs (TDM, SDIO), PSRAM init on v2, and USB host enumeration needs real boards on the bench — the structure and call sites are in place; the values are clearly marked.

The legacy v1/v2 firmware is preserved on `main` and on the `RP235x*` branches. Nothing was removed from there.

---

<p align="center"><sub>BSD-3-Clause · made with care for people who like their synths to sound exactly like they wanted</sub></p>
