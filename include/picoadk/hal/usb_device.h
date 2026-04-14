// USB composite device.
//
// Pick which USB classes appear when the board enumerates. Anything from
// none to all-of-the-above:
//
//     UsbConfig u;
//     u.classes = UsbClass::Cdc | UsbClass::Midi | UsbClass::Msc;
//     u.msc_backend = UsbMscBackend::SdCard;
//     u.midi_cables = 4;
//     picoadk::Usb::configure(u);
//
// Defaults preserve the v1 firmware behaviour (USB MIDI, single cable).

#pragma once

#include <cstdint>

namespace picoadk {

enum class UsbClass : uint8_t {
    None = 0,
    Cdc  = 1u << 0,    // CDC ACM serial — also routes stdio when enabled
    Midi = 1u << 1,    // USB-MIDI (1..16 virtual cables)
    Msc  = 1u << 2,    // Mass-storage
    All  = Cdc | Midi | Msc,
};
inline constexpr UsbClass operator|(UsbClass a, UsbClass b) {
    return static_cast<UsbClass>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
inline constexpr bool any(UsbClass mask, UsbClass bit) {
    return (static_cast<uint8_t>(mask) & static_cast<uint8_t>(bit)) != 0;
}

enum class UsbMscBackend : uint8_t {
    None       = 0,
    FlashFs    = 1,    // internal flash via LittleFS/SPIFFS region
    SdCard     = 2,    // SD card via Storage HAL
    Both       = 3,    // exposed as two LUNs (LUN0 = flash, LUN1 = SD)
};

struct UsbConfig {
    UsbClass       classes      = UsbClass::Midi;
    uint8_t        midi_cables  = 1;            // 1..16 virtual cables when Midi enabled
    UsbMscBackend  msc_backend  = UsbMscBackend::None;
    bool           cdc_routes_stdio = true;     // when CDC enabled, hook printf to it
    const char*    manufacturer = "DatanoiseTV";
    const char*    product      = "PicoADK";
    uint16_t       vid          = 0x2E8A;
    uint16_t       pid          = 0x104B;
};

namespace Usb {

// Apply config. Must be called *before* System::init() (or before tusb_init()
// if you skip System::init), because TinyUSB descriptors are generated from
// this state at device-startup time.
bool configure(const UsbConfig& cfg);

const UsbConfig& config();

// Hot-mount/unmount the SD LUN so audio can keep streaming while host is
// scanning. When unmounted, host sees "media not present" and won't
// interfere with concurrent SD reads from the firmware.
void msc_set_sd_present(bool present);

}  // namespace Usb
}  // namespace picoadk
