// Storage HAL — SD card access.
//
// On PicoADK v2 the card is wired to the RP2350 over a 4-bit SDIO bus driven
// by PIO; the underlying transport hits ~20 MB/s. On v1 we fall back to SPI.
// Either way the high-level API exposes a SdFat (FAT32 / exFAT, long file
// names) filesystem so user code stays the same.
//
// Direct SdFat objects are exposed for users that need streaming I/O for
// sample loading; convenience wrappers cover the simple read/write case.

#pragma once

#include <cstddef>
#include <cstdint>

// Forward-declared so this header doesn't drag SdFat into every translation
// unit. Implementation includes <SdFat.h>.
class SdFs;
class FsFile;

namespace picoadk {

enum class StorageBus : uint8_t { Auto, Spi, Sdio };

struct StorageConfig {
    StorageBus  bus       = StorageBus::Auto;   // Auto = best for current board
    uint32_t    clock_khz = 0;                   // 0 = max safe (50 MHz SDIO / 25 MHz SPI)
    bool        require_long_filenames = true;
};

namespace Storage {

bool init(const StorageConfig& cfg = {});
bool mount();
void unmount();
bool is_mounted();

// Direct access to the SdFat object (FAT32 + exFAT + LFN out of the box).
SdFs& fs();

// Convenience read-the-whole-file-into-buffer; returns bytes read or 0.
std::size_t read_file(const char* path, void* dst, std::size_t max_bytes);

// Convenience write-buffer-as-file; truncates existing.
bool write_file(const char* path, const void* src, std::size_t bytes);

uint64_t card_size_bytes();
const char* fs_type_name();      // "FAT32" / "exFAT" / "FAT16"

}  // namespace Storage
}  // namespace picoadk
