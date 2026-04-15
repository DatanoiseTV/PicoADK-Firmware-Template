# Architecture

This document explains how the v3 firmware fits together — useful when adding a new HAL subsystem, a new DSP module, or trying to understand why the audio task wakes when it does.

## Layered model

```
┌────────────────────────────────────────────────────────────────┐
│                       Application                              │   examples/, your code
│                       Hardware::init() + Audio::set_callback() │
├────────────────────────────────────────────────────────────────┤
│                       picoadk::dsp::*                          │   include/picoadk/dsp/
│   modules · sample_player · pd · math · Real (Q16/float)       │
├────────────────────────────────────────────────────────────────┤
│                       picoadk::* HAL                           │   include/picoadk/hal/
│   Audio · Midi · Controls · Storage · Psram · Display · gfx    │
│   Encoders · Buttons · Mux · Usb · UsbHost · VoiceLink · Tasks │
├────────────────────────────────────────────────────────────────┤
│                       Pico SDK 2.2 · pico-extras · TinyUSB     │   lib/pico-sdk/
│                       FreeRTOS-Kernel (RPi SMP fork)           │   lib/FreeRTOS-Kernel/
├────────────────────────────────────────────────────────────────┤
│                       Hardware                                 │   RP2040 / RP2350
└────────────────────────────────────────────────────────────────┘
```

Application code never reaches into the SDK directly — it talks to the picoadk:: HAL. That's what lets one source tree run on both boards.

## Audio pipeline

```
                                 ┌────────────────┐
   I2S TX pin  ◀────────  PIO ◀──┤ DMA out chain  │◀── int32 ring buffers
   I2S BCK/WS ◀────────  PIO     │ A → B → A → B  │
                                 └────────────────┘
                                        ▲
                                        │ deinterleave
                                        │ (audio_task)
                                        │
   ┌──────────────────┐                 │      float planar
   │ user audio_cb()  │ ───────────────►│   ┌──────────────┐
   └──────────────────┘                 │   │ g_planar_in  │
                                        │   │ g_planar_out │
   I2S RX pin  ────────►  PIO ──────────┤   └──────────────┘
                                        ▼
                                 ┌────────────────┐
                                 │ DMA in chain   │
                                 │ A → B → A → B  │
                                 └────────────────┘
```

Two PIO state machines (output, input) run independently when the user picks `Direction::Duplex`. Two DMA channels per direction chase each other forever via `chain_to`. On every chain hop the IRQ fires, flips a `g_active_*` index, and notifies the audio task. The audio task does the int32↔float conversion and runs the user callback off-IRQ — so DSP work is never racing PIO refills.

Latency = `block_size / sample_rate` per buffer. With `block_size = 32` at 48 kHz that's ~0.67 ms.

## Threading model

```
        priority    task              what it does
        ────────    ────              ────────────
        MAX-1       audio (built-in)  off-IRQ user audio callback
        MAX-1       MIDI              tud_task() + UART byte pump + Midi::process()
        MAX-2       USBHOST (opt.)    tuh_task()
        IDLE+2      MUX               background ADC mux scan
        IDLE+1      WDOG              watchdog tickle
        IDLE        idle hook         vPortIdleTask
```

All tasks coexist on both cores (SMP). `xTaskCreatePinnedToCore()` (the ESP-IDF-style alias) lets you pin time-critical tasks to one core — keep audio on core 0, push USB / MIDI / display refresh to core 1.

## Memory map

```
RP2040 (v1)                          RP2350 (v2)
─────────────                        ─────────────
Flash      4 MB QSPI                 Flash       4 MB QSPI
SRAM     264 KB                      SRAM      520 KB
                                     PSRAM       8 MB QSPI
                                                 ├─ .psram_data    (KEEP'd by linker)
                                                 ├─ .psram_bss     (KEEP'd by linker)
                                                 └─ .psram_dynamic (TLSF heap)
```

On v2, anything tagged with `PICOADK_PSRAM_BSS` / `PICOADK_PSRAM_DATA` lands in the linker-reserved sections at boot. The remainder is fed to the in-tree TLSF allocator (`src/third_party/tlsf/`); `Psram::alloc` and `placement-new (PSRAM)` use it.

DSP modules that own large buffers (`DelayLine::configure_auto`, `FdnReverb::configure`, `MemorySampleSource`) **try PSRAM first** and fall back to the system heap if PSRAM isn't available — the same code runs on both boards.

## MIDI dispatch

```
                  ┌──────────┐
   USB device  ── ▶│          │
   UART (DIN) ── ▶ │ Midi::   │── ▶ user::on_note_on / on_cc / on_pb / …
   USB host   ── ▶ │ dispatch │── ▶ midi::Handler (NRPN/RPN aggregation)
                  │          │── ▶ user::on_raw (catch-all for sysex etc.)
                  └──────────┘
```

Each transport runs in `Midi::process()` (called from a MIDI task). Inbound messages from any source land in the same callback set; `MidiMessage::source` reports which transport delivered it.

## VoiceLink (multi-board)

```
   Master                Slave 1               Slave 2
   ─────────────────     ─────────────────     ─────────────────
   audio cb              audio cb              audio cb
       │                     ▲                     ▲
       │ note_on(ch,n,v) → allocator picks slot
       │                  if slot==0: local
       ▼                  else: send_frame ───┐
   UART TX   ──────►     UART RX ─────────────┴──► dispatch into slave's
                         UART TX ──────────► Slave 2 (chain pass-through)
                                              ▲
                                              └── enumeration on boot reports
                                                  voices_local + board_serial
                                                  back to master
```

The control path is UART; the audio path stays local on every board. Frame protocol is documented at the top of `src/hal/voicelink.cpp`. CRC-8/SMBus protects every frame.

## File-by-file map

| File | Role |
|---|---|
| `boards/picoadk_v{1,2}.h`            | Pin map + feature flags. Selected by `PICOADK_BOARD`. |
| `boards/picoadk_board.h`             | Selector — includes the right per-board header. |
| `boards/datanoisetv_rp{2040,2350}_dsp.h` | Pico-SDK board headers (defaults for stdio/SPI/I²C). |
| `boards/picoadk_v2_psram.ld`         | v2 linker fragment defining `.psram_data/.psram_bss/__psram_dynamic_*`. |
| `cmake/picoadk/board.cmake`          | Board selector + PICO_PLATFORM + PICO_BOARD. |
| `cmake/picoadk/freertos.cmake`       | Picks RPi RP2040 or RP2350_ARM_NTZ SMP port. |
| `cmake/picoadk/vult.cmake`           | `picoadk_add_vult_dsp()` codegen helper. |
| `cmake/picoadk/sdio.cmake`           | FetchContent for rabbitholecomputing/SDIO_RP2350. |
| `cmake/picoadk/sdfat.cmake`          | FetchContent for greiman/SdFat. |
| `cmake/picoadk/libpd.cmake`          | FetchContent for libpd/libpd. |
| `include/picoadk/picoadk.h`          | Umbrella header. |
| `include/picoadk/hal/*.h`            | Public HAL surface — one per subsystem. |
| `include/picoadk/dsp/*.h`            | DSP types and modules. |
| `include/vultin.h`                   | Vult runtime ABI (Q16 helpers + adc128_read_vult). |
| `src/hal/*.cpp`                      | HAL implementations. |
| `src/hal/pio/*.pio`                  | I2S, TDM, SDIO, encoder PIO programs. |
| `src/midi/*.cpp`                     | MIDI dispatcher + transports + handlers + Launchpad. |
| `src/usb/*.{c,cpp}`                  | TinyUSB descriptors + composite-device glue. |
| `src/dsp/*.cpp`                      | DSP primitives + sampler + libpd bridge. |
| `src/third_party/tlsf/`              | In-tree TLSF allocator (used by Psram). |
| `examples/<n>_<name>/main.cpp`       | Build with `-DPICOADK_APP=examples/<n>_<name>`. |
