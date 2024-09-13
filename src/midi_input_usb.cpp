#include "midi_input_usb.h"
#include "tusb.h" // Assuming you're using TinyUSB

// Setters for the various MIDI callbacks
void MIDIInputUSB::setNoteOnCallback(void (*callback)(uint8_t, uint8_t, uint8_t)) {
    MIDINoteOnCallback = callback;
}

void MIDIInputUSB::setNoteOffCallback(void (*callback)(uint8_t, uint8_t, uint8_t)) {
    MIDINoteOffCallback = callback;
}

void MIDIInputUSB::setCCCallback(void (*callback)(uint8_t, uint8_t, uint8_t)) {
    MIDICCCallback = callback;
}

void MIDIInputUSB::setPitchBendCallback(void (*callback)(uint16_t, uint8_t)) {
    MIDIPitchBendCallback = callback;
}

void MIDIInputUSB::setAftertouchCallback(void (*callback)(uint8_t, uint8_t)) {
    MIDIAftertouchCallback = callback;
}

void MIDIInputUSB::setChannelPressureCallback(void (*callback)(uint8_t, uint8_t)) {
    MIDIChannelPressureCallback = callback;
}

// Helper function to parse pitch bend data
uint16_t MIDIInputUSB::parsePitchBend(const uint8_t* packet) {
    // Combine the two data bytes into a 14-bit pitch bend value
    return (packet[2] << 7) | packet[1];
}

// macro for reading midi packets. in usb host mode, tuh_midi_stream_read is used instead, which is a wrapper for this function
#if (OPT_MODE_HOST == 1)
#define TINYUSB_MIDI_STREAM_READ(packet, len) tuh_midi_stream_read(packet, len)
#define TINYUSB_MIDI_STREAM_AVAILABLE() tuh_midi_available()
#error "We are in host mode"
#else
#define TINYUSB_MIDI_STREAM_READ(packet, len) tud_midi_stream_read(packet, len)
#define TINYUSB_MIDI_STREAM_AVAILABLE() tud_midi_available()
#endif

void MIDIInputUSB::process() {
    uint8_t packet[4];

    while (TINYUSB_MIDI_STREAM_AVAILABLE()) {
        TINYUSB_MIDI_STREAM_READ(packet, 3);
        #ifdef DEBUG_MIDI
        printf("%02X %02X %02X %02X\n", packet[0], packet[1], packet[2], packet[3]);
        #endif

        uint8_t status = packet[0] >> 4;
        uint8_t channel = packet[0] & 0xF;

        switch (status) {
            case 0x8: // Note Off
                if (MIDINoteOffCallback != nullptr) {
                    MIDINoteOffCallback(packet[1], packet[2], channel);
                }
                break;

            case 0x9: // Note On
                if (MIDINoteOnCallback != nullptr) {
                    MIDINoteOnCallback(packet[1], packet[2], channel);
                }
                break;

            case 0xB: // Control Change
                if (MIDICCCallback != nullptr) {
                    MIDICCCallback(packet[1], packet[2], channel);
                }
                break;

            case 0xE: // Pitch Bend
                if (MIDIPitchBendCallback != nullptr) {
                    uint16_t pitchBendValue = parsePitchBend(packet);
                    MIDIPitchBendCallback(pitchBendValue, channel);
                }
                break;

            case 0xA: // Polyphonic Aftertouch
                if (MIDIAftertouchCallback != nullptr) {
                    MIDIAftertouchCallback(packet[1], packet[2]);
                }
                break;

            case 0xD: // Channel Pressure (Aftertouch)
                if (MIDIChannelPressureCallback != nullptr) {
                    MIDIChannelPressureCallback(packet[1], channel);
                }
                break;

            default:
                // Handle other types of MIDI messages if needed
                break;
        }
    }
}
