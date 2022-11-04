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

#include "oled.h"

#define USE_DIN_MIDI 1
#define DEBUG_MIDI 1
audio_buffer_pool_t *ap;

Dsp_process_type ctx;

MIDIInputUSB midi_input_usb;

#ifdef __cplusplus
extern "C"
{
#endif

    volatile long tick_start;
    volatile long tick_end;

    volatile long dsp_start;
    volatile long dsp_end;

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
            if (watchdog_caused_reboot()) {
              printf("!!! WATCHDOG CAUSED REBOOT !!!\n");
            }
            watchdog_update();
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }

    void oled_task(void *p)
    {

        // I2C is "open drain", pull ups to keep signal high when no data is being
        // sent
        i2c_init(i2c_default, 400 * 1000);
        gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
        gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
        gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

        char *words[] = {"RP2040", "DSP", "DEMO"};

        ssd1306_t disp;
        disp.external_vcc = false;
        ssd1306_init(&disp, 128, 32, 0x3C, i2c0);
        ssd1306_invert(&disp, true);
        ssd1306_clear(&disp);

        while (1)
        {
            for (int i = 0; i < sizeof(words) / sizeof(char *); ++i)
            {
                ssd1306_draw_string(&disp, 8, 10, 2, words[i]);
                ssd1306_show(&disp);
                vTaskDelay(pdMS_TO_TICKS(1000));
                ssd1306_clear(&disp);
            }
        }
    }

    // MIDI callbacks
    void note_on_callback(uint8_t note, uint8_t level, uint8_t channel)
    {
        Dsp_noteOn(ctx, note, level, channel);
#ifdef DEBUG_MIDI
        printf("note on (ch %d): %d %d\n", channel, note, level);
#endif
    }

    void note_off_callback(uint8_t note, uint8_t level, uint8_t channel)
    {
        Dsp_noteOff(ctx, note, channel);
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
        midi_input_usb.setCCCallback(cc_callback);
        midi_input_usb.setNoteOnCallback(note_on_callback);
        midi_input_usb.setNoteOffCallback(note_off_callback);

        while (1)
        {
            tick_start = to_us_since_boot(get_absolute_time());
            tud_task();
            midi_input_usb.process();
            tick_end = to_us_since_boot(get_absolute_time());
        }
    }

    int main(void)
    {
        vreg_set_voltage(VREG_VOLTAGE_1_30);
        sleep_ms(1);
        set_sys_clock_khz(226000, true);
        sleep_ms(1);

        board_init();
        tusb_init();
        stdio_init_all();

        // set gpio 25 (soft mute) to output and set to 1 (unmute)
        gpio_init(25);
        gpio_set_dir(25, GPIO_OUT);
        gpio_put(25, 1);

        // set gpio 23 (deemphasis) to output and set to 1 (enable)
        gpio_init(23);
        gpio_set_dir(23, GPIO_OUT);
        gpio_put(23, 1);

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

        xTaskCreate(usb_midi_task, "USBMIDI", 8192, NULL, configMAX_PRIORITIES, NULL);
        //xTaskCreate(print_task, "TaskList", 4096, NULL, configMAX_PRIORITIES - 1, NULL);
        //xTaskCreate(oled_task, "OLED", 10240, NULL, configMAX_PRIORITIES - 1, NULL);
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

        dsp_start = to_us_since_boot(get_absolute_time());
        for (uint i = 0; i < buffer->max_sample_count; i++)
        {
            int32_t smp = Dsp_process(ctx, 0) << 16u;
            smp = smp <= -0x8000 ? -0x8000 : (smp > 0x7fff ? 0x7fff : smp);
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
