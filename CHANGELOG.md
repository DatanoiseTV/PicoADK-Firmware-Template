# Changelog

## v3.0.0 — 2026

A complete from-scratch rewrite of the PicoADK firmware template. Same hardware, brand-new HAL, both boards from one source tree.

### Added
- **Dual-board build** (`-DPICOADK_BOARD=v1_rp2040|v2_rp2350`).
- **Audio HAL** with runtime sample rate (8 k–192 k), bit depth (16/24/32), block size, direction (output/input/full-duplex), and frame format (I²S/LeftJust/RightJust/TDM4/TDM8). PIO + DMA engine, off-IRQ FreeRTOS audio task.
- **MIDI HAL** with simultaneous USB device + UART + USB host transports and a unified callback set. General handler with NRPN/RPN aggregation, 14-bit pitch bend, MIDI clock + transport. Launchpad programmer-mode driver with diff-only updates.
- **USB composite device** assembled at runtime — CDC + USB-MIDI + MSC in any combination. MSC exposes internal flash and/or SD card as separate LUNs.
- **USB host** via pico_pio_usb on a second port; native port stays in device mode.
- **Controls HAL** — MCP3208 SPI ADC, internal SAR ADC, mux banks (CD4051/74HC4067) intermixed with direct inputs, debounced buttons (1 kHz timer), polled quadrature encoders.
- **Display HAL** — u8g2-backed driver for SSD1306/SH1106/SSD1309 over I²C or SPI; `gfx::Canvas` C++ wrapper for the common drawing calls.
- **PSRAM** — TLSF allocator, linker-reserved sections, `placement-new (PSRAM)`. Delay and FDN reverb auto-allocate from PSRAM when available.
- **Storage** — SdFat with FAT32/exFAT/LFN. SDIO 4-bit on v2 via `rabbitholecomputing/SDIO_RP2350`, SPI on v1.
- **DSP toolkit** — `dsp::Real` adapts (float on RP2350 FPU, Q16 fixed on RP2040). Hardware-accelerated helpers (RP2040 hw divider + interp, M33 DSP intrinsics). Module library: BLEP/wavetable/noise oscillators, ADSR, state-variable + ladder filters, saturator, wavefolder, delay, FDN reverb, compressor.
- **Sampler** — WAV loader (16/24/32-bit PCM, mono/stereo), pitch shift, looping, multisample key-mapping, optional time-stretch.
- **SoundFont 2** playback via TinySoundFont (full multisample / loop / release / modulators), loaded from PSRAM-resident memory or SD.
- **SFZ subset** parser (regions, groups, control default_path; lokey/hikey/key/pitch_keycenter/lovel/hivel/transpose/tune/volume/loop_*) — each region opens a streaming sample source for high polyphony.
- **Streaming polyphony** — per-`StreamingSampleSource` 4096-frame prefetch ring (PSRAM-backed when available); sequential reads hit the cache, only note-on triggers an SD seek.
- **PureData** — optional libpd integration (`-DPICOADK_LIBPD=ON`).
- **VoiceLink** — multi-board polyphony over 3 Mbaud UART. Auto-enumeration; per-card polyphony capacity; allocation policies (round-robin, oldest, quietest).
- **FreeRTOS SMP** on both boards (Raspberry Pi's curated kernel fork). `xTaskCreatePinnedToCore()` ESP32-style alias.
- **Stdio routing** to UART (default) and USB-CDC (when enabled).
- **VS Code support** — `CMakePresets.json`, `.vscode/{settings,launch,tasks,extensions,cmake-kits}.json` compatible with the official Raspberry Pi Pico extension.
- Examples: `00_template` (copy-and-edit scaffold), `01_passthrough` (minimal duplex), `02_synth_vult` (original Vult monosynth on the new HAL).

### Submodules pinned
- pico-sdk **2.2.0**
- pico-extras **sdk-2.2.0**
- raspberrypi/FreeRTOS-Kernel — SMP ports for both boards
- pico-pio-usb (latest)
- u8g2 (latest)

### Removed
- Legacy `src/main.cpp`, `picoadk_hw.{h,cpp}`, `midi_input_usb.{h,cpp}`, `arduino_compat.h`, `helper.h`, `lib/audio`, `lib/oled`, the DatanoiseTV `pico-extras` fork, and the `dummy` file. The old behaviour is reproduced by `examples/02_synth_vult` against the new HAL.

### Migration
The legacy firmware still lives on `main` and on tags. v3 lives on `v3-refactor` and supersedes the entire `src/` layout. Vult DSP files (`vultsrc/dsp.vult`, `vultsrc/filter.vult`) are unchanged.
