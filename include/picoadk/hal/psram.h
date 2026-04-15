// PSRAM HAL.
//
// On PicoADK v2 (RP2350) an 8 MB QSPI PSRAM is mapped into the address space
// by the QMI. The linker reserves a `.psram_bss` and `.psram_data` section so
// you can place large buffers there with the PSRAM_BSS / PSRAM_DATA macros.
//
// For dynamic allocation we run a TLSF heap on top of the remaining PSRAM,
// matching the API arduino-pico exposes (psram_malloc / psram_free, plus a
// `new (PSRAM)` placement-new). On v1 (RP2040) all of these fall back to the
// system heap so user code compiles either way.

#pragma once

#include <cstddef>
#include <new>

namespace picoadk {

namespace Psram {

// True on v2 (and once init() has succeeded).
bool available();

// Bring up the QMI, probe the chip, mount the TLSF heap. Idempotent.
bool init();

// Sizes refer to the heap area only (linker-reserved sections are excluded).
std::size_t total_bytes();
std::size_t free_bytes();
std::size_t largest_free_block();

void* alloc(std::size_t bytes, std::size_t alignment = 8);
void  free(void* ptr);

}  // namespace Psram

// Tag type for placement-new allocation in PSRAM.
struct PsramTag {};
inline constexpr PsramTag PSRAM{};

}  // namespace picoadk

// `Foo* f = new (picoadk::PSRAM) Foo(...);`
inline void* operator new(std::size_t bytes, picoadk::PsramTag) {
    return picoadk::Psram::alloc(bytes);
}
inline void* operator new[](std::size_t bytes, picoadk::PsramTag) {
    return picoadk::Psram::alloc(bytes);
}
inline void operator delete(void* p, picoadk::PsramTag) noexcept {
    picoadk::Psram::free(p);
}
inline void operator delete[](void* p, picoadk::PsramTag) noexcept {
    picoadk::Psram::free(p);
}

// Section attributes for static placement.
#if PICOADK_HAS_PSRAM
#  define PICOADK_PSRAM_BSS  __attribute__((section(".psram_bss")))
#  define PICOADK_PSRAM_DATA __attribute__((section(".psram_data")))
#else
#  define PICOADK_PSRAM_BSS
#  define PICOADK_PSRAM_DATA
#endif
