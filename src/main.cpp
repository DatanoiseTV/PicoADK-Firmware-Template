#include <stdio.h>
#include "project_config.h"


// Reverse compatibility with old PicoSDK.
#if __has_include("bsp/board_api.h")
#include "bsp/board_api.h"
#else
#include "bsp/board.h"
#endif

#include "midi_input_usb.h"
#include "audio_subsystem.h"
#include "picoadk_hw.h"

#include "FreeRTOS.h"
#include <task.h>
#include <queue.h>

#include "arduino_compat.h"

// Audio Buffer (Size is set in lib/audio/include/audio_subsystem.h)
audio_buffer_pool_t *ap;

MIDIInputUSB usbmidi;

#ifdef __cplusplus
extern "C"
{
#endif

    // MIDI callbacks

    void note_off_callback(uint8_t note, uint8_t vel, uint8_t channel)
    {
        // put your node off code for the dsp engine here
        // e.g. Dsp_noteOff(ctx, note, channel);
    }

    void note_on_callback(uint8_t note, uint8_t vel, uint8_t channel)
    {
        if (vel > 0)
        {
            // put your node on code for the dsp engine here
            // e.g. Dsp_noteOn(ctx, note, channel);
        }
        else
        {
            note_off_callback(note, vel, channel);
        }
    }

    void cc_callback(uint8_t cc, uint8_t value, uint8_t channel)
    {
        // put your cc code for the dsp engine here
        // e.g. Dsp_cc(ctx, cc, value, channel);
    }

    // This task processes the USB MIDI input
    void usb_midi_task(void *pvParameters)
    {

        // Basic Callbacks for MIDI Notes & CCs
        usbmidi.setCCCallback(cc_callback);
        usbmidi.setNoteOnCallback(note_on_callback);
        usbmidi.setNoteOffCallback(note_off_callback);

        while (1)
        {
            // TinyUSB Device Task
            tud_task();
            usbmidi.process();
        }
    }

    // This task blinks the LED on GPIO 2
    void blinker_task(void *pvParameters)
    {
        gpio_init(2);
        gpio_set_dir(2, GPIO_OUT);

        while (1)
        {
            gpio_put(2, 1);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_put(2, 0);
        }
    }

    int main(void)
    {
        // initialize the hardware
        picoadk_init();

        // Initialize your DSP engine here

        // Initialize the audio subsystem
        ap = init_audio();

        // Create FreeRTOS Tasks for USB MIDI and printing statistics
        xTaskCreate(usb_midi_task, "USBMIDI", 4096, NULL, configMAX_PRIORITIES, NULL);
        xTaskCreate(blinker_task, "BLINKER", 128, NULL, configMAX_PRIORITIES - 1, NULL);

        // Start the scheduler.
        vTaskStartScheduler();

        // Idle loop.
        while (1)
        {
            ;
            ;
        }
    }

    // This fis the I2S callback function. It is called when the I2S subsystem
    // needs more audio data. It is called at a fixed rate.
    // The audio data is stored in the audio_buffer_t struct.
    //
    // !!!! Please note that this function needs to take less to execute than the time
    // between two calls to this function. Otherwise the audio will be interrupted. !!!!

    void __not_in_flash_func(i2s_callback_func())
    {
        audio_buffer_t *buffer = take_audio_buffer(ap, false);
        if (buffer == NULL)
        {
            return;
        }
        int32_t *samples = (int32_t *)buffer->buffer->bytes;
        // We are filling the buffer with 32-bit samples (2 channels)
        for (uint i = 0; i < buffer->max_sample_count; i++)
        {
            samples[i * 2 + 0] = 0;   // Use your DSP function here to output the L sample
            samples[i * 2 + 1] = 0;   // Use your DSP function here to output the R sample

        }

        buffer->sample_count = buffer->max_sample_count;
        give_audio_buffer(ap, buffer);
        return;
    }

#ifdef __cplusplus
}
#endif
