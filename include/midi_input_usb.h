#ifndef __MIDI_INPUT_USB__H__
#define __MIDI_INPUT_USB__H__


#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "project_config.h"
#include "tusb.h"

class MIDIInputUSB
{
public:
    MIDIInputUSB() {

    }

    void process();

    void setNoteOnCallback(void (*callback)(uint8_t, uint8_t, uint8_t));
    void setNoteOffCallback(void (*callback)(uint8_t, uint8_t, uint8_t));
    void setCCCallback(void (*callback)(uint8_t, uint8_t, uint8_t));

private:
    const char MIDICH = 1;

    char MIDIRunningStatus;
    char MIDINote;
    char MIDILevel;

    void (*MIDINoteOnCallback)(uint8_t, uint8_t, uint8_t);
    void (*MIDINoteOffCallback)(uint8_t, uint8_t, uint8_t);
    void (*MIDICCCallback)(uint8_t, uint8_t, uint8_t);

    void midi_task();

};

#endif // __MIDI_INPUT_USB_H__
