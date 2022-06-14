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
    void usb_midi_task(void *pvParameters){
        tud_task();
        midi_input_usb.process();
    }

    int main(void)
    {
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

        xTaskCreate(usb_midi_task, "USBMIDI", 8192, NULL, configMAX_PRIORITIES-1, NULL);
        vTaskStartScheduler();

        while(1){
            ;;
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
