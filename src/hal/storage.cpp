// Storage HAL — SD card access.
//
// This file owns the public Storage namespace and bridges to the platform-
// specific backend (SDIO 4-bit on v2 via PIO; SPI on v1).
//
// Phase 4: SdFat is vendored as lib/SdFat (added on first build via the
// helper in cmake/picoadk/sdfat.cmake). The wrapper exposes a SdFs instance
// with FAT32/exFAT and long file names enabled. Until SdFat lands the
// backend reports "not mounted" so apps can compile and run.

#include "picoadk/hal/storage.h"
#include "picoadk_board.h"

#include <cstring>

namespace picoadk::Storage {

namespace {
StorageConfig g_cfg{};
bool          g_inited  = false;
bool          g_mounted = false;
uint64_t      g_card_size = 0;
}

bool init(const StorageConfig& cfg) {
    g_cfg    = cfg;
    g_inited = true;
    // Phase 4: bring up the chosen bus (SDIO PIO on v2, SPI on v1) and
    // probe the card. For now we report no card.
    return false;
}

bool mount() {
    // Phase 4: instantiate SdFs and call begin().
    return false;
}

void unmount() { g_mounted = false; }
bool is_mounted() { return g_mounted; }

SdFs& fs() {
    static SdFs* dummy = nullptr;
    return *dummy;
}

std::size_t read_file(const char*, void*, std::size_t)        { return 0; }
bool        write_file(const char*, const void*, std::size_t) { return false; }
uint64_t    card_size_bytes()                                  { return g_card_size; }
const char* fs_type_name()                                     { return g_mounted ? "FAT32" : "none"; }

}  // namespace picoadk::Storage
