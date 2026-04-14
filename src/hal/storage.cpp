// Storage HAL stub — Phase 4 wires SdFat in for real (FAT32/exFAT + LFN, SDIO
// on v2, SPI on v1). For Phase 0 we expose the API surface and report the
// card as "not mounted" so user code can compile.

#include "picoadk/hal/storage.h"

namespace picoadk::Storage {

namespace { bool g_mounted = false; }

bool init(const StorageConfig&) { return false; }
bool mount()                    { return false; }
void unmount()                  { g_mounted = false; }
bool is_mounted()               { return g_mounted; }

SdFs& fs() {
    // Will be replaced with a real SdFs instance in Phase 4.
    static SdFs* dummy = nullptr;
    return *dummy;
}

std::size_t read_file(const char*, void*, std::size_t)        { return 0; }
bool        write_file(const char*, const void*, std::size_t) { return false; }
uint64_t    card_size_bytes()                                  { return 0; }
const char* fs_type_name()                                     { return "none"; }

}  // namespace picoadk::Storage
