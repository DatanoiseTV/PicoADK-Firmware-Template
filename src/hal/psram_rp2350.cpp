// PSRAM HAL — RP2350 backend.
//
// pico-sdk 2.x brings up the QMI-attached PSRAM during startup when the
// board file declares PICO_RP2350_PSRAM_CS. The chip ends up memory-mapped
// at PSRAM_BASE (0x11000000) with the size pico-sdk auto-detected.
//
// We carve the chip into two regions:
//   * static area  — anything placed via PICOADK_PSRAM_BSS / PICOADK_PSRAM_DATA
//                    by the linker script (`.psram_bss`, `.psram_data`).
//   * dynamic area — the remainder, fed to a TLSF allocator.
//
// The linker script (boards/picoadk_v2_psram.ld) defines `__psram_dynamic_start`
// / `__psram_dynamic_end` for us; we initialise TLSF over that range.

#include "picoadk/hal/psram.h"
#include "tlsf.h"

#include "pico/stdlib.h"

#include <cstddef>
#include <cstdint>

extern "C" {
extern uint8_t __psram_dynamic_start[];
extern uint8_t __psram_dynamic_end[];
}

namespace picoadk::Psram {

namespace {
tlsf_t      g_tlsf  = nullptr;
std::size_t g_total = 0;
}

bool available() { return g_tlsf != nullptr; }

bool init() {
    if (g_tlsf) return true;
    std::size_t bytes = (std::size_t)(__psram_dynamic_end - __psram_dynamic_start);
    if (bytes < 4096) return false;     // PSRAM not detected by pico-sdk
    g_tlsf  = tlsf_create_with_pool(__psram_dynamic_start, bytes);
    g_total = bytes;
    return g_tlsf != nullptr;
}

std::size_t total_bytes()        { return g_total; }
std::size_t free_bytes()         { return g_tlsf ? tlsf_free_bytes(g_tlsf) : 0; }
std::size_t largest_free_block() { return g_tlsf ? tlsf_largest_free_block(g_tlsf) : 0; }

void* alloc(std::size_t bytes, std::size_t align) {
    if (!g_tlsf) return nullptr;
    return tlsf_aligned_alloc(g_tlsf, align, bytes);
}
void free(void* p) { if (g_tlsf && p) tlsf_free(g_tlsf, p); }

}  // namespace picoadk::Psram
