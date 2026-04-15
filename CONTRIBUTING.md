# Contributing

Glad you're here. The aim of this template is to be **the most pleasant way to build pro-quality audio firmware for the PicoADK family**. Help us keep it that way.

## Quick rules

- One source tree, two boards. Anything you add must build for both `v1_rp2040` and `v2_rp2350` (board-conditional code is fine; missing-on-v1 features should fall back gracefully, not error).
- HAL APIs are public surface — they go in `include/picoadk/hal/`. Implementations under `src/hal/`. Vendored deps under `src/third_party/` or as submodules.
- Test on real hardware before merging anything that touches PIO, DMA, USB, PSRAM, or SDIO.
- Match the style of nearby code — short comments explaining *why*, not *what*.

## Building

```sh
git submodule update --init --recursive
cmake --preset v1 && cmake --build build/v1
cmake --preset v2 && cmake --build build/v2
```

Both must succeed before opening a PR.

## Adding a new HAL subsystem

1. Public header → `include/picoadk/hal/<thing>.h` (struct config, functions/namespace).
2. Implementation → `src/hal/<thing>.cpp` (no globals where avoidable; RAII when state has lifetime).
3. Wire into `src/hal/hardware.cpp`'s `Hardware::init()` if it should auto-start.
4. Add a one-paragraph entry to the README "What it gives you" table.
5. If the subsystem needs a CMake-fetched dependency, put it in `cmake/picoadk/<dep>.cmake` behind a `-DPICOADK_ENABLE_<DEP>=ON` flag.

## Adding a new DSP module

1. Class declaration → `include/picoadk/dsp/modules.h`.
2. Implementation → `src/dsp/modules.cpp`.
3. Use `picoadk::dsp::Real` as the sample type so the module compiles for both boards.
4. If the module owns a buffer ≥ 4 KB, prefer `Psram::alloc` when available — RP2350's SRAM is precious.

## Commit messages

- Imperative ("add X", not "added X").
- First line ≤72 characters; body wrapped at 80.
- Mention which subsystem changed (`audio:`, `midi:`, `psram:`, `cmake:`).

No trailers (no `Co-Authored-By`, no auto-generated signatures).

## Filing issues

Please include:
- Board (v1 or v2).
- pico-sdk version (`cat lib/pico-sdk/CMakeLists.txt | grep VERSION_STRING`).
- Toolchain version (`arm-none-eabi-gcc --version`).
- The smallest reproducer you can.
