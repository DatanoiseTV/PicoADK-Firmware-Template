#include "midi_input_usb.h"

void MIDIInputUSB::setNoteOnCallback(void (*callback)(uint8_t, uint8_t, uint8_t))
{
    MIDINoteOnCallback = callback;
}

void MIDIInputUSB::setNoteOffCallback(void (*callback)(uint8_t, uint8_t, uint8_t))
{
    MIDINoteOffCallback = callback;
}

void MIDIInputUSB::setCCCallback(void (*callback)(uint8_t, uint8_t, uint8_t))
{
    MIDICCCallback = callback;
}

void MIDIInputUSB::process()
{
    uint8_t packet[4];
    while (tud_midi_available())
    {
        tud_midi_stream_read(packet, 3);

        #ifdef DEBUG_MIID
        printf("%02X %02X %02X %02X\n", packet[0], packet[1], packet[2], packet[3]);
        #endif

        if (packet[0] >> 4  == 0x8 || packet[0] >> 4 == 0x9)
        {
            if (packet[0] >> 4 == 0x8)
            {
                if (MIDINoteOffCallback != NULL)
                {
                    MIDINoteOffCallback(packet[1], packet[2], packet[0] & 0xF);
                }
            }
            else if(packet[0] >> 4 == 0x9)
            {
                if (MIDINoteOnCallback != NULL)
                {
                    MIDINoteOnCallback(packet[1], packet[2], packet[0] & 0xF);
                }
            }

        }
        else if (packet[0] >> 4 == 0xB)
        {
            if (MIDICCCallback != NULL)
            {
                MIDICCCallback(packet[1], packet[2], packet[0] & 0xF);
            }
        }
    }
}
