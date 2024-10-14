#include <stdio.h>
#include "project_config.h"

#include "midi_input_usb.h"
#include "audio_subsystem.h"
#include "picoadk_hw.h"

#include "FreeRTOS.h"
#include <task.h>
#include <queue.h>

#include "arduino_compat.h"

#include <psram.h>

volatile size_t psram_size;

// Audio Buffer (Size is set in lib/audio/include/audio_subsystem.h)
audio_buffer_pool_t *audio_pool;

#ifdef __cplusplus
extern "C"
{
#endif

    inline int getTotalPSRAMHeap() {
#if defined(RP2350_PSRAM_CS)
        extern size_t __psram_total_space();
        return __psram_total_space();
#else
        return 0;
#endif
    }

     inline int getUsedPSRAMHeap() {
#if defined(RP2350_PSRAM_CS)
        extern size_t __psram_total_used();
        return __psram_total_used();
#else
        return 0;
#endif
    }


     inline int getFreePSRAMHeap() {
        return getTotalPSRAMHeap() - getUsedPSRAMHeap();
    }

    /**
     * Task to blink an LED on GPIO pin 2.
     *
     * @param pvParameters Unused task parameters
     */
    void blinker_task(void *pvParameters)
    {
        gpio_init(2);
        gpio_set_dir(2, GPIO_OUT);

        while (1)
        {
            gpio_put(2, 1);
            vTaskDelay(pdMS_TO_TICKS(50)); // Delay for 100ms
            gpio_put(2, 0);
            vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100ms

            printf("Total PSRAM Heap: %d\n", getFreePSRAMHeap());
        }
    }



    /**
     * Main entry point.
     */
    int main(void)
    {
        // Initialize hardware
        picoadk_init();


        // Initialize DSP engine (if needed)

        // Initialize the audio subsystem
        audio_pool = init_audio();

        // Create FreeRTOS tasks for MIDI handling and LED blinking

        //xTaskCreate(blinker_task, "Blinker_Task", 128, NULL, configMAX_PRIORITIES - 1, NULL);

        // Start the FreeRTOS scheduler
       //vTaskStartScheduler();

        // Idle loop (this is fine for Cortex-M33)
        while (1)
        {
            // Could use `taskYIELD()` or similar here if needed
            printf("Total PSRAM Heap: %d\n", getTotalPSRAMHeap());
            sleep_ms(1000);
        }
    }

    /**
     * I2S audio callback for filling the audio buffer with samples.
     *
     * This function is called at a fixed rate determined by the audio subsystem
     * and must return within the interval between calls to avoid audio glitches.
     */
    void __not_in_flash_func(i2s_callback_func())
    {
        audio_buffer_t *buffer = take_audio_buffer(audio_pool, false);
        if (buffer == NULL)
        {
            return;
        }

        int32_t *samples = (int32_t *)buffer->buffer->bytes;

        // Fill buffer with 32-bit samples (stereo, 2 channels)
        for (uint i = 0; i < buffer->max_sample_count; i++)
        {
            samples[i * 2 + 0] = 0; // Left channel sample
            samples[i * 2 + 1] = 0; // Right channel sample
            // Use your DSP function here for generating the audio samples
        }

        buffer->sample_count = buffer->max_sample_count;
        give_audio_buffer(audio_pool, buffer);
    }

#ifdef __cplusplus
}
#endif