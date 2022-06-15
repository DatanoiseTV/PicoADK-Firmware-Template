#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/interp.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"

#include "project_config.h"
#include "bsp/board.h"
#include "midi_input_usb.h"

#include "audio_subsystem.h"
#include "vult.h"

#include "FreeRTOS.h"
#include <task.h>
#include <queue.h>

#define USE_DIN_MIDI 1
audio_buffer_pool_t *ap;

Dsp_process_type ctx;

MIDIInputUSB midi_input_usb;

#ifdef __cplusplus
extern "C"
{
#endif

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
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }

    // MIDI callbacks
void note_on_callback(uint8_t note, uint8_t level, uint8_t channel)
{
    Dsp_noteOn(ctx, note, level, channel);
    printf("note on (ch %d): %d %d\n", channel, note, level);

}

void note_off_callback(uint8_t note, uint8_t level, uint8_t channel)
{
    Dsp_noteOff(ctx, note, channel);
    printf("note off (ch %d): %d %d\n", channel, note, level);
}

void cc_callback(uint8_t cc, uint8_t value, uint8_t channel)
{
    Dsp_controlChange(ctx, cc, value, channel);
    //printf("cc (ch %d): %d %d\n", channel, cc, value);
}

    void usb_midi_task(void *pvParameters)
    {
        midi_input_usb.setCCCallback(cc_callback);
        midi_input_usb.setNoteOnCallback(note_on_callback);
        midi_input_usb.setNoteOffCallback(note_off_callback);

        while(1){
            tud_task();
            midi_input_usb.process();
        }
    }

    int main(void)
    {
        vreg_set_voltage(VREG_VOLTAGE_1_30);
        sleep_ms(1);
        set_sys_clock_khz(400000, true);
        sleep_ms(1);

        board_init();
        tusb_init();
        stdio_init_all();

        // Initialize Vult DSP. This must match the DSP code.
        Dsp_process_init(ctx);
        Dsp_default_init(ctx);
        Dsp_default(ctx);

        // Initialize all Trigger inputs
        for (int pin = PIN_TRIG_IN_0; pin <= PIN_TRIG_BTN; pin++)
        {
            gpio_init(pin);
            gpio_pull_up(pin);
            gpio_set_dir(pin, 0);
            gpio_set_input_hysteresis_enabled(pin, false);
        }

        ap = init_audio();

        xTaskCreate(usb_midi_task, "USBMIDI", 8192, NULL, configMAX_PRIORITIES - 1, NULL);
        xTaskCreate(print_task, "TaskList", 4096, NULL, configMAX_PRIORITIES -1 , NULL);
        vTaskStartScheduler();

        while (1)
        {
            ;
            ;
        }
    }

    void i2s_callback_func()
    {
        audio_buffer_t *buffer = take_audio_buffer(ap, false);
        if (buffer == NULL)
        {
            return;
        }
        int32_t *samples = (int32_t *)buffer->buffer->bytes;

        for (uint i = 0; i < buffer->max_sample_count; i++)
        {
            int32_t smp = Dsp_process(ctx, 0) << 16u;
            samples[i * 2 + 0] = smp; // LEFT
            samples[i * 2 + 1] = smp; // RIGHT
        }
        buffer->sample_count = buffer->max_sample_count;
        give_audio_buffer(ap, buffer);
        return;
    }

#ifdef __cplusplus
}
#endif
