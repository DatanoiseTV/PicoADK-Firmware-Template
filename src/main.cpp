#include <stdio.h>
#include "project_config.h"
#include "bsp/board.h"
#include "midi_input_usb.h"
#include "audio_subsystem.h"
#include "vult.h"
#include "picoadk_hw.h"

#include "FreeRTOS.h"
#include <task.h>
#include <queue.h>

#define USE_DIN_MIDI 1
#define DEBUG_MIDI 1

audio_buffer_pool_t *ap;
Dsp_process_type ctx;

MIDIInputUSB usbmidi;

#ifdef __cplusplus
extern "C"
{
#endif

    volatile long dsp_start;
    volatile long dsp_end;

    // This task prints the statistics about the running FreeRTOS tasks
    // and how long it takes for the I2S callback to run.
    void print_task(void *p)
    {
        char ptrTaskList[2048];
        while (1)
        {
            vTaskList(ptrTaskList);
            printf("\033[2J");
            printf("\033[0;0HTask\t\tState\tPrio\tStack\tNum\n%s\n", ptrTaskList);
            printf("======================================================\n");
            printf("B = Blocked, R = Ready, D = Deleted, S = Suspended\n");
            printf("Milliseconds since boot: %d\n", xTaskGetTickCount() * portTICK_PERIOD_MS);
            printf("dsp task took %d uS\n", dsp_end - dsp_start);
            watchdog_update();
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }

    // MIDI callbacks
    void note_on_callback(uint8_t note, uint8_t level, uint8_t channel)
    {
        if(level > 0){
            Dsp_noteOn(ctx, note, level, channel);
            gpio_put(15, 1);
        #ifdef DEBUG_MIDI
                printf("note on (ch %d): %d %d\n", channel, note, level);
        #endif
        } else {
            Dsp_noteOff(ctx, note, channel);
            gpio_put(15, 0);
        #ifdef DEBUG_MIDI
                printf("note off (ch %d): %d %d\n", channel, note, level);
        #endif
        }
    }

    void note_off_callback(uint8_t note, uint8_t level, uint8_t channel)
    {
        Dsp_noteOff(ctx, note, channel);
        gpio_put(15, 0);
        #ifdef DEBUG_MIDI
                printf("note off (ch %d): %d %d\n", channel, note, level);
        #endif
    }

    void cc_callback(uint8_t cc, uint8_t value, uint8_t channel)
    {
        Dsp_controlChange(ctx, cc, value, channel);
        #ifdef DEBUG_MIDI
                printf("cc (ch %d): %d %d\n", channel, cc, value);
        #endif
    }

    void usb_midi_task(void *pvParameters)
    {
        usbmidi.setCCCallback(cc_callback);
        usbmidi.setNoteOnCallback(note_on_callback);
        usbmidi.setNoteOffCallback(note_off_callback);

        while (1)
        {
            tud_task();
            usbmidi.process();
        }
    }

    void blinker_task(void *pvParameters)
    {
        // set gpio 2-5 and 15 as outputs
        for(int i = 2; i < 6; i++) {
            gpio_init(i);
            gpio_set_dir(i, GPIO_OUT);
        }

        while (1)
        {
            // chase leds on gpio 2-5
            for(int i = 2; i < 6; i++) {
                gpio_put(i, 1);
                vTaskDelay(pdMS_TO_TICKS(15));
                gpio_put(i, 0);
            }
        }
    }

    int main(void)
    {
        // initialize the hardware
        picoadk_init();

        // Initialize Vult DSP context.
        Dsp_process_init(ctx);
        Dsp_default_init(ctx);
        Dsp_default(ctx);

        // Initialize the audio subsystem
        ap = init_audio();

        // Create FreeRTOS Tasks for USB MIDI and printing statistics
        xTaskCreate(usb_midi_task, "USBMIDI", 8192, NULL, configMAX_PRIORITIES, NULL);
        xTaskCreate(print_task, "TASKLIST", 4096, NULL, configMAX_PRIORITIES - 1, NULL);
        xTaskCreate(blinker_task, "BLINKER", 4096, NULL, configMAX_PRIORITIES - 1, NULL);
        vTaskStartScheduler();

        // Idle loop. 
        while (1)
        {
            ;
            ;
        }
    }

    // This function is called by the audio subsystem when it needs more audio data.
    void i2s_callback_func()
    {
        audio_buffer_t *buffer = take_audio_buffer(ap, false);
        if (buffer == NULL)
        {
            return;
        }
        int32_t *samples = (int32_t *)buffer->buffer->bytes;

        dsp_start = to_us_since_boot(get_absolute_time());

        // We are filling the buffer with 32-bit samples (2 channels)
        for (uint i = 0; i < buffer->max_sample_count; i++)
        {
            // smp should be the output of your processing code.
            // In case of the Vult Example, this is Dsp_process(ctx, 0);
            int32_t smp = Dsp_process(ctx, 0) << 16u;
            samples[i * 2 + 0] = smp; // LEFT
            samples[i * 2 + 1] = smp; // RIGHT
        }

        dsp_end = to_us_since_boot(get_absolute_time());

        buffer->sample_count = buffer->max_sample_count;
        give_audio_buffer(ap, buffer);
        return;
    }

#ifdef __cplusplus
}
#endif
