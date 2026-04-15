// USB MSC backend — exposes either the internal flash (LittleFS region) or
// the SD card (via Storage HAL) as a USB mass-storage device. When both are
// enabled the device shows up with two LUNs.
//
// TinyUSB calls into here from its MSC class driver; we route to the
// configured backend.

#include "picoadk/hal/usb_device.h"
#include "picoadk/hal/storage.h"

#include "tusb.h"
#include "pico/stdlib.h"

#include <cstring>

namespace {

bool      g_sd_present = true;
constexpr uint32_t kBlockSize = 512;

uint8_t lun_count() {
    using namespace picoadk;
    auto m = Usb::config().msc_backend;
    return m == UsbMscBackend::Both ? 2 : (m == UsbMscBackend::None ? 0 : 1);
}

bool lun_is_sd(uint8_t lun) {
    using namespace picoadk;
    auto m = Usb::config().msc_backend;
    if (m == UsbMscBackend::Both) return lun == 1;
    return m == UsbMscBackend::SdCard;
}

}  // namespace

namespace picoadk::Usb { void msc_set_sd_present(bool p) { g_sd_present = p; } }

extern "C" {

uint8_t tud_msc_get_maxlun_cb(void) {
    uint8_t n = lun_count();
    return n ? (uint8_t)(n - 1) : 0;
}

void tud_msc_inquiry_cb(uint8_t lun, uint8_t vid[8], uint8_t pid[16], uint8_t rev[4]) {
    const char* p = lun_is_sd(lun) ? "SD-Card " : "Flash   ";
    memcpy(vid, "PicoADK ", 8);
    memcpy(pid, p, 8); memset(pid + 8, ' ', 8);
    memcpy(rev, "1.0 ", 4);
}

bool tud_msc_test_unit_ready_cb(uint8_t lun) {
    if (lun_is_sd(lun)) return g_sd_present && picoadk::Storage::is_mounted();
    return true;
}

void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size) {
    *block_size = kBlockSize;
    if (lun_is_sd(lun)) {
        uint64_t bytes = picoadk::Storage::card_size_bytes();
        *block_count = bytes ? (uint32_t)(bytes / kBlockSize) : 0;
    } else {
        // Flash backend not yet implemented — advertise zero capacity so the
        // host doesn't try to read.
        *block_count = 0;
    }
}

bool tud_msc_start_stop_cb(uint8_t lun, uint8_t /*power_condition*/, bool start, bool /*load_eject*/) {
    if (lun_is_sd(lun) && !start) picoadk::Storage::unmount();
    return true;
}

int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
    (void)offset; (void)lba; (void)buffer;
    if (lun_is_sd(lun) && picoadk::Storage::is_mounted()) {
        // Wired up properly in Phase 4 once SdFat is in.
        return (int32_t)bufsize;
    }
    return -1;
}

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
    (void)offset; (void)lba; (void)buffer;
    if (lun_is_sd(lun) && picoadk::Storage::is_mounted()) {
        return (int32_t)bufsize;
    }
    return -1;
}

int32_t tud_msc_scsi_cb(uint8_t /*lun*/, uint8_t const /*scsi_cmd*/[16], void* /*buffer*/, uint16_t /*bufsize*/) {
    return -1;   // unhandled SCSI commands fall through with sense data
}

}  // extern "C"
