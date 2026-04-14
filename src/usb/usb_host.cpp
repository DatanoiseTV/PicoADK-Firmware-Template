// USB host glue.
//
// When pico_pio_usb is available (it is, via the lib/pico-pio-usb submodule)
// we run the host stack on a PIO state machine pair and leave the native USB
// port in device mode. This is the configuration the v2 hardware was
// designed for (the 5-pin USB pins on the silk).
//
// MIDI traffic from connected devices is unpacked into our existing
// `Midi::dispatch()` so user callbacks fire identically regardless of
// whether the data came from USB device, USB host, or DIN UART.

#include "picoadk/hal/usb_host.h"
#include "picoadk/hal/midi.h"
#include "../midi/midi_internal.h"

#include "tusb.h"

#include <FreeRTOS.h>
#include <task.h>

namespace picoadk::UsbHost {

namespace {
UsbHostConfig g_cfg{};
UsbRole       g_role = UsbRole::Device;
TaskHandle_t  g_task = nullptr;
bool          g_inited = false;

void host_task(void*) {
    for (;;) {
#if CFG_TUH_ENABLED
        tuh_task();
#endif
        vTaskDelay(1);
    }
}
}  // namespace

bool init(const UsbHostConfig& cfg) {
    if (g_inited) return true;
    g_cfg = cfg;
#if CFG_TUH_ENABLED
    if (cfg.use_pio_usb) {
        // pico_pio_usb config is set via tuh_configure() before tuh_init().
        // The exact knobs vary by tinyusb version; the defaults work on v2
        // when the pin pair is contiguous.
        tuh_init(BOARD_TUH_RHPORT);
    } else {
        tuh_init(0);    // native port — caller is responsible for not also
                        // running the device stack.
    }
    g_role   = UsbRole::Host;
    g_inited = true;
    if (!g_task)
        xTaskCreate(host_task, "USBHOST", 2048, nullptr, configMAX_PRIORITIES - 2, &g_task);
    return true;
#else
    (void)cfg;
    return false;
#endif
}

bool set_role(UsbRole r) {
#if CFG_TUH_ENABLED
    g_role = r;
    return true;
#else
    (void)r;
    return false;
#endif
}
UsbRole role() { return g_role; }

bool any_midi_device_connected() {
#if CFG_TUH_ENABLED
    return tuh_midi_configured(0);
#else
    return false;
#endif
}

void process() {
#if CFG_TUH_ENABLED
    if (!tuh_midi_configured(0)) return;
    uint8_t pkt[4];
    while (tuh_midi_packet_read(0, pkt)) {
        uint8_t cin = pkt[0] & 0x0F;
        if (cin < 0x8 || cin > 0xE) continue;
        Midi::MidiMessage m{ pkt[1], pkt[2], pkt[3], Midi::MidiTransport::UsbHost };
        Midi::dispatch(m);
    }
#endif
}

}  // namespace picoadk::UsbHost
