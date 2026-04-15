// USB composite device — public API + descriptor-side glue.

#include "picoadk/hal/usb_device.h"
#include "get_serial.h"

#include "pico/stdlib.h"
#include "tusb.h"

#include <cstring>

namespace picoadk::Usb {

namespace {
UsbConfig g_cfg{};
}

bool configure(const UsbConfig& cfg) {
    g_cfg = cfg;
    usb_serial_init();
    return true;
}
const UsbConfig& config() { return g_cfg; }

}  // namespace picoadk::Usb

// ---- C glue read by usb_descriptors.c -----------------------------------
extern "C" {

const char* picoadk_usb_manufacturer_str() { return picoadk::Usb::config().manufacturer; }
const char* picoadk_usb_product_str()      { return picoadk::Usb::config().product; }
uint16_t    picoadk_usb_vid()              { return picoadk::Usb::config().vid; }
uint16_t    picoadk_usb_pid() {
    // Twiddle the low 4 bits so each composite combination presents a
    // distinct PID — saves Windows from caching the wrong driver.
    return (uint16_t)(picoadk::Usb::config().pid ^
                      (uint16_t)(static_cast<uint8_t>(picoadk::Usb::config().classes) & 0x0F));
}
uint8_t     picoadk_usb_class_mask()       { return static_cast<uint8_t>(picoadk::Usb::config().classes); }
uint8_t     picoadk_usb_midi_cables()      { return picoadk::Usb::config().midi_cables; }

}  // extern "C"
