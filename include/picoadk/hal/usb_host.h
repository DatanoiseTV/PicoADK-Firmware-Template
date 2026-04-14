// USB host — for plugging in MIDI keyboards, Launchpads, USB-MIDI controllers.
//
// The RP2040/RP2350 native USB port is single-role hardware: at any instant
// it acts as a device OR a host, not both. Two integration modes:
//
//   1. Compile-time role        — set PICOADK_USB_ROLE=host (or device, drd).
//                                   `drd` lets you call set_role() at runtime
//                                   to swap; the USB stack is torn down and
//                                   re-initialised, which briefly disconnects.
//
//   2. Second-port PIO host     — pico_pio_usb runs a host stack on PIO pins
//                                   (board-defined PICOADK_PIN_PIO_USB_DP) so
//                                   the native port can stay in device mode.
//                                   Recommended on PicoADK v2 thanks to the
//                                   spare 5-pin USB header.
//
// MIDI from connected devices is delivered through the existing Midi
// callbacks with `MidiTransport::UsbHost`; nothing else in user code changes.

#pragma once

#include <cstdint>

namespace picoadk {

enum class UsbRole : uint8_t {
    Device,
    Host,
    Drd,            // dual-role; switchable at runtime
};

struct UsbHostConfig {
    bool        use_pio_usb        = true;     // run host on PIO so device port stays up
    uint8_t     pio_usb_dp_pin     = 16;       // PIO USB D+ (D- = D+ + 1). Pick a free pair.
    bool        midi_class_enabled = true;
    bool        hid_class_enabled  = false;    // future: HID gamepads / pedals
    bool        cdc_class_enabled  = false;    // future: serial → MIDI bridges
};

namespace UsbHost {

bool init(const UsbHostConfig& cfg = {});

// Runtime role switch (only valid when built with PICOADK_USB_ROLE=drd or
// when use_pio_usb=true — the latter never moves the device port). Returns
// false if the build doesn't support it.
bool set_role(UsbRole role);
UsbRole role();

// True once at least one MIDI device is enumerated.
bool any_midi_device_connected();

// Pump the host stack. Call from a task (≥1 kHz is fine).
void process();

}  // namespace UsbHost
}  // namespace picoadk
