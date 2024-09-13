#ifndef MIDI_INPUT_USB_H
#define MIDI_INPUT_USB_H

#include <functional>
#include <stdint.h>

class MIDIInputUSB {
public:
    // Callback setters
    void setNoteOnCallback(void (*callback)(uint8_t, uint8_t, uint8_t));
    void setNoteOffCallback(void (*callback)(uint8_t, uint8_t, uint8_t));
    void setCCCallback(void (*callback)(uint8_t, uint8_t, uint8_t));
    void setPitchBendCallback(void (*callback)(uint16_t, uint8_t));
    void setAftertouchCallback(void (*callback)(uint8_t, uint8_t));
    void setChannelPressureCallback(void (*callback)(uint8_t, uint8_t));

    // MIDI processing function
    void process();

private:
    // Callback function pointers using std::function for flexibility
    std::function<void(uint8_t, uint8_t, uint8_t)> MIDINoteOnCallback = nullptr;
    std::function<void(uint8_t, uint8_t, uint8_t)> MIDINoteOffCallback = nullptr;
    std::function<void(uint8_t, uint8_t, uint8_t)> MIDICCCallback = nullptr;
    std::function<void(uint16_t, uint8_t)> MIDIPitchBendCallback = nullptr;
    std::function<void(uint8_t, uint8_t)> MIDIAftertouchCallback = nullptr;
    std::function<void(uint8_t, uint8_t)> MIDIChannelPressureCallback = nullptr;

    // Helper function for parsing pitch bend data
    uint16_t parsePitchBend(const uint8_t* packet);
};

#endif // MIDI_INPUT_USB_H
