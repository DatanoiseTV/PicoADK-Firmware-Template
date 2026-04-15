// USB-MIDI transport via TinyUSB.

#include "picoadk/hal/midi.h"
#include "midi_internal.h"

#include "tusb.h"

namespace picoadk::Midi {

bool usb_init() {
    // tusb_init() is called from System::init(). Nothing more to do here.
    return true;
}

void usb_process() {
    tud_task();
    uint8_t pkt[4];
    while (tud_midi_available() && tud_midi_packet_read(pkt)) {
        // Code Index Number is in low nibble of byte 0; we only forward
        // channel-voice messages (CIN 0x8..0xE) here.
        uint8_t cin = pkt[0] & 0x0F;
        if (cin < 0x8 || cin > 0xE) continue;
        MidiMessage m{ pkt[1], pkt[2], pkt[3], MidiTransport::Usb };
        dispatch(m);
    }
}

void usb_send(const uint8_t* bytes, std::size_t len) {
    if (!tud_midi_mounted()) return;
    tud_midi_stream_write(0, bytes, static_cast<uint32_t>(len));
}

}  // namespace picoadk::Midi
