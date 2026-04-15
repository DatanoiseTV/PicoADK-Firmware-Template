# Overclocking

The audio template can run well past stock clock rates. DSP-heavy apps (big
reverb graphs, convolution, high-polyphony synths) benefit directly — each
extra MHz is more MACs per audio block.

## The one-line version

```cpp
picoadk::System::set_clock_khz(300'000);    // on v2 — 2× the stock rate
```

The HAL handles voltage, flash timing and any registered retune callbacks
automatically. No need to mess with `vreg_set_voltage()` or
`set_sys_clock_khz()` directly.

## Voltage ladders

The HAL walks a per-board ladder and picks the lowest vreg step that's
known-good for the requested frequency.

### PicoADK v1 — RP2040

| Clock            | Voltage   | Notes |
|---               |---        |---    |
| ≤ 133 MHz        | 1.10 V    | Stock |
| ≤ 225 MHz        | 1.15 V    |       |
| ≤ 275 MHz        | 1.20 V    |       |
| ≤ 350 MHz        | 1.25 V    |       |
| ≤ **420 MHz**    | 1.30 V    | Community daily-driver for audio |

### PicoADK v2 — RP2350

| Clock            | Voltage   | Notes |
|---               |---        |---    |
| ≤ 150 MHz        | 1.10 V    | Stock |
| ≤ 225 MHz        | 1.15 V    |       |
| ≤ 275 MHz        | 1.20 V    |       |
| ≤ **300 MHz**    | 1.25 V    | Max recommended with PSRAM |

Values err conservatively. Above these the silicon lottery kicks in — some
chips will run fine, others won't. If you clock past the ladder,
`set_clock_khz()` refuses rather than guessing a voltage.

## Runtime retune

Subsystems whose timing depends on `sys_clk` register a callback so they
can recompute their dividers when the clock changes:

```cpp
System::on_clock_changed([](uint32_t new_khz) {
    my_i2s_retune(new_khz);
});
```

The HAL ships its own callbacks for:

- **PSRAM QMI timing** (v2) — recomputed against the new clock so reads
  stay timing-safe after an overclock.
- **I²S PIO clkdiv** — recomputed so BCK stays at `sample_rate × slot_count × slot_width` regardless of the new sys_clk.
- **SPI ADC divisor** — MCP3208 clock stays ≤ 20 MHz.

## Going the other way — underclocking

`set_clock_khz()` also handles drops: it lowers the clock first, *then*
drops voltage. Useful for battery-powered builds or to keep the board cool
when only MIDI passthrough is needed. Down to 48 MHz still keeps 48 kHz
audio running comfortably on v2.

## Gotchas

- **USB at exotic clocks.** TinyUSB needs its USB PLL at 48 MHz. pico-sdk
  arranges this via `clk_usb` which is independent of `clk_sys`; any
  sensible sys_clk is fine.
- **Flash at high clocks.** pico-sdk's `set_sys_clock_khz()` adjusts
  `PICO_FLASH_SPI_CLKDIV` internally; no extra work on our end.
- **First boot can't overclock.** If you call `set_clock_khz()` *before*
  `Hardware::init()` / `System::init()`, you bypass the boot sequence. Always init first, then retune.
- **PSRAM init happens at pico-sdk boot time** using the *initial* clock.
  If you overclock, call `Psram::retune()` (coming with the arduino-pico
  PSRAM adoption) — or just overclock first-thing in `main()` before any
  PSRAM traffic.

## Example

```cpp
#include "picoadk/picoadk.h"

using namespace picoadk;

int main() {
    Hardware::init({});            // boots at default

    if (!System::set_clock_khz(280'000)) {
        // Requested rate above the safety cap — stay at default.
    }

    // ... rest of app ...
}
```
