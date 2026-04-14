// Storage HAL — bridges the Storage namespace to SdFat.
//
// Wiring rules:
//   * If `-DPICOADK_ENABLE_SDFAT=ON` (sets PICOADK_HAS_SDFAT) the real
//     implementation runs against SdFat's `SdFs` object.
//   * If `-DPICOADK_ENABLE_SDIO=ON` is also set on the v2 build
//     (PICOADK_HAS_SDIO_RP2350=1) we mount through the
//     rabbitholecomputing/SDIO_RP2350 driver — full 4-bit speed.
//   * Otherwise we fall back to SPI through pico-sdk's hardware_spi.
//
// When SdFat isn't compiled in, every entry point returns "not mounted" so
// apps stay compileable and just skip SD-dependent paths gracefully.

#include "picoadk/hal/storage.h"
#include "picoadk_board.h"

#include <cstring>

#if PICOADK_HAS_SDFAT
#  include "SdFat.h"
#endif

namespace picoadk::Storage {

namespace {

StorageConfig g_cfg{};
bool          g_inited    = false;
bool          g_mounted   = false;
uint64_t      g_card_size = 0;

#if PICOADK_HAS_SDFAT
SdFs        g_fs;
const char* g_fs_type = "none";

bool sdfat_begin_spi() {
    uint32_t mhz = g_cfg.clock_khz ? (g_cfg.clock_khz / 1000) : 25;
    SdSpiConfig c(PICOADK_PIN_ADC_CS, DEDICATED_SPI, SD_SCK_MHZ(mhz));
    return g_fs.begin(c);
}

bool sdfat_begin_sdio() {
#  if PICOADK_HAS_SDIO_RP2350
    return g_fs.begin(SdioConfig(FIFO_SDIO));
#  else
    return false;
#  endif
}
#endif

}  // namespace

bool init(const StorageConfig& cfg) {
    g_cfg    = cfg;
    g_inited = true;
#if PICOADK_HAS_SDFAT
    return mount();
#else
    return false;
#endif
}

bool mount() {
#if PICOADK_HAS_SDFAT
    StorageBus bus = g_cfg.bus;
    if (bus == StorageBus::Auto) {
#  if PICOADK_HAS_SDIO && PICOADK_HAS_SDIO_RP2350
        bus = StorageBus::Sdio;
#  else
        bus = StorageBus::Spi;
#  endif
    }
    g_mounted = (bus == StorageBus::Sdio) ? sdfat_begin_sdio() : sdfat_begin_spi();
    if (g_mounted) {
        g_card_size = (uint64_t)g_fs.card()->sectorCount() * 512ull;
        switch (g_fs.fatType()) {
            case FAT_TYPE_FAT16: g_fs_type = "FAT16"; break;
            case FAT_TYPE_FAT32: g_fs_type = "FAT32"; break;
            case FAT_TYPE_EXFAT: g_fs_type = "exFAT"; break;
            default:             g_fs_type = "unknown"; break;
        }
    }
    return g_mounted;
#else
    return false;
#endif
}

void unmount() {
#if PICOADK_HAS_SDFAT
    if (g_mounted) g_fs.end();
#endif
    g_mounted   = false;
    g_card_size = 0;
}

bool is_mounted() { return g_mounted; }

#if PICOADK_HAS_SDFAT
SdFs& fs() { return g_fs; }
#else
SdFs& fs() {
    // SdFat wasn't compiled in. Apps must check is_mounted() before calling
    // fs(); reaching this is a programming error — trap loudly.
    static char dummy[1] = {0};
    __asm volatile("bkpt #0");
    return *reinterpret_cast<SdFs*>(dummy);
}
#endif

std::size_t read_file(const char* path, void* dst, std::size_t max_bytes) {
#if PICOADK_HAS_SDFAT
    if (!g_mounted) return 0;
    FsFile f;
    if (!f.open(path, O_RDONLY)) return 0;
    int n = f.read(dst, (uint32_t)max_bytes);
    f.close();
    return n > 0 ? (std::size_t)n : 0;
#else
    (void)path; (void)dst; (void)max_bytes;
    return 0;
#endif
}

bool write_file(const char* path, const void* src, std::size_t bytes) {
#if PICOADK_HAS_SDFAT
    if (!g_mounted) return false;
    FsFile f;
    if (!f.open(path, O_WRONLY | O_CREAT | O_TRUNC)) return false;
    int n = f.write(src, (uint32_t)bytes);
    f.close();
    return (std::size_t)n == bytes;
#else
    (void)path; (void)src; (void)bytes;
    return false;
#endif
}

uint64_t    card_size_bytes() { return g_card_size; }
const char* fs_type_name()    {
#if PICOADK_HAS_SDFAT
    return g_fs_type;
#else
    return "none";
#endif
}

}  // namespace picoadk::Storage
