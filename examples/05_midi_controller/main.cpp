// =============================================================================
//  PicoADK v3 — Hardware MIDI Controller
// -----------------------------------------------------------------------------
//  No audio. The board enumerates as a USB-MIDI device and forwards changes
//  on its 4 pots + 1 encoder + 1 button as MIDI CCs / notes.
//
//      pots  0..3    →  CC 20, 21, 22, 23   (channel 1)
//      encoder       →  CC 24, signed (relative)
//      encoder btn   →  CC 25, momentary
//      trig button   →  Note 60, vel 100 (toggle)
//
//  Demonstrates the Controls / Encoders / Buttons HAL together; useful as a
//  starting point for hardware-only USB-MIDI controllers.
// =============================================================================

#include "picoadk/picoadk.h"

#include <FreeRTOS.h>
#include <task.h>

using namespace picoadk;

namespace {

constexpr uint8_t kChannel = 0;

EncoderPins g_enc_pins[1];
ButtonPins  g_btn_pins[2];

uint8_t  g_last_pot[4] = {0xFF, 0xFF, 0xFF, 0xFF};
int32_t  g_last_enc    = 0;
bool     g_last_enc_btn = false;
bool     g_note_held    = false;

void poll_controls() {
    for (uint8_t i = 0; i < 4; ++i) {
        uint8_t v = (uint8_t)(Controls::read_adc8(i) >> 5);    // 12-bit → 7-bit
        if (v != g_last_pot[i]) {
            Midi::send_cc(kChannel, (uint8_t)(20 + i), v);
            g_last_pot[i] = v;
        }
    }
    EncoderReading enc;
    Encoders::poll(&enc, 1);
    if (enc.delta != 0) {
        // Encode signed delta as 0x40 ± n (relative bin offset, common in DAWs).
        int v = 0x40 + enc.delta;
        if (v < 0) v = 0; if (v > 0x7F) v = 0x7F;
        Midi::send_cc(kChannel, 24, (uint8_t)v);
    }
    if (enc.button_pressed)  Midi::send_cc(kChannel, 25, 0x7F);
    if (enc.button_released) Midi::send_cc(kChannel, 25, 0x00);

    ButtonReading btns[2];
    Buttons::poll(btns, 2);
    if (btns[0].event == ButtonEvent::Pressed) {
        g_note_held = !g_note_held;
        if (g_note_held) Midi::send_note_on(kChannel, 60, 100);
        else             Midi::send_note_off(kChannel, 60);
        Controls::set_note_led(g_note_held);
    }
}

[[noreturn]] void controls_task(void*) {
    for (;;) {
        poll_controls();
        Midi::process();
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}
[[noreturn]] void watchdog_task(void*) {
    for (;;) { System::feed_watchdog(); vTaskDelay(pdMS_TO_TICKS(500)); }
}

}  // namespace

int main() {
    UsbConfig usb;
    usb.classes = UsbClass::Cdc | UsbClass::Midi;       // CDC for debug, MIDI for data
    usb.product = "PicoADK Controller";
    Usb::configure(usb);

    HardwareConfig hw;
    hw.init_audio = false;             // no audio
    hw.midi.inputs  = MidiTransport::Usb;
    hw.midi.outputs = MidiTransport::Usb;
    Hardware::init(hw);

    g_enc_pins[0] = { PICOADK_PIN_ENC_A, PICOADK_PIN_ENC_B, PICOADK_PIN_ENC_BTN };
    Encoders::init(g_enc_pins, 1);

    g_btn_pins[0] = { PICOADK_PIN_TRIG_BTN };
    g_btn_pins[1] = { PICOADK_PIN_ENC_BTN };
    Buttons::init(g_btn_pins, 2);

    xTaskCreate(controls_task,  "CTRL", 1024, nullptr, configMAX_PRIORITIES - 2, nullptr);
    xTaskCreate(watchdog_task,  "WDOG", 256,  nullptr, tskIDLE_PRIORITY + 1,     nullptr);
    vTaskStartScheduler();
    for (;;) __asm volatile("wfi");
}
