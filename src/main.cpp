#include <stdio.h>
#include "project_config.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

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

audio_buffer_pool_t *audio_pool;

#include "Heavy_prog.hpp"
Heavy_prog pd_prog(SAMPLE_RATE);

float smp[2];

#if (USE_USB_MIDI_HOST == 1)
static uint8_t midi_dev_addr = 0;
#endif

MIDIInputUSB usbMIDI;

#define HV_HASH_NOTEIN          0x67E37CA3
#define HV_HASH_CTLIN           0x41BE0f9C
#define HV_HASH_POLYTOUCHIN     0xBC530F59
#define HV_HASH_PGMIN           0x2E1EA03D
#define HV_HASH_TOUCHIN         0x553925BD
#define HV_HASH_BENDIN          0x3083F0F7
#define HV_HASH_MIDIIN          0x149631bE
#define HV_HASH_MIDIREALTIMEIN  0x6FFF0BCF

#define HV_HASH_NOTEOUT         0xD1D4AC2
#define HV_HASH_CTLOUT          0xE5e2A040
#define HV_HASH_POLYTOUCHOUT    0xD5ACA9D1
#define HV_HASH_PGMOUT          0x8753E39E
#define HV_HASH_TOUCHOUT        0x476D4387
#define HV_HASH_BENDOUT         0xE8458013
#define HV_HASH_MIDIOUT         0x6511DE55
#define HV_HASH_MIDIOUTPORT     0x165707E4

#define MIDI_RT_CLOCK           0xF8
#define MIDI_RT_START           0xFA
#define MIDI_RT_CONTINUE        0xFB
#define MIDI_RT_STOP            0xFC
#define MIDI_RT_ACTIVESENSE     0xFE
#define MIDI_RT_RESET           0xFF

#ifdef __cplusplus
extern "C" {
#endif

void usb_midi_task(void *pvParameters) {
    usbMIDI.setCCCallback([](uint8_t cc, uint8_t value, uint8_t channel) {
        pd_prog.sendMessageToReceiverV(HV_HASH_CTLIN, 0, "fff", (float)value, (float)cc, (float)channel);
    });

    usbMIDI.setNoteOnCallback([](uint8_t note, uint8_t velocity, uint8_t channel) {
        if (velocity > 0) {
            pd_prog.sendMessageToReceiverV(HV_HASH_NOTEIN, 0, "fff", (float)note, (float)velocity, (float)channel);
        } else {
            pd_prog.sendMessageToReceiverV(HV_HASH_NOTEIN, 0, "fff", (float)note, (float)velocity, (float)channel);
        }
    });

    usbMIDI.setNoteOffCallback([](uint8_t note, uint8_t velocity, uint8_t channel) {
        pd_prog.sendMessageToReceiverV(HV_HASH_NOTEIN, 0, "fff", (float)note, (float)velocity, (float)channel);
    });

    while (1) {
        #if (USE_USB_MIDI_HOST == 1)
        tuh_task();
        #else
        tud_task();
        #endif
        usbMIDI.process();
    }
}

void blinker_task(void *pvParameters) {
    gpio_init(2);
    gpio_set_dir(2, GPIO_OUT);

    while (1) {
        gpio_put(2, 1);
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100ms
        gpio_put(2, 0);
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100ms
    }
}

int main(void) {
    // Initialize hardware
    picoadk_init();

    // Initialize UART0
    //uart_init(uart0, 115200);
    //gpio_set_function(12, GPIO_FUNC_UART);
    //gpio_set_function(13, GPIO_FUNC_UART);

    //stdio_init_all();

    // Initialize DSP engine (if needed)
    
    // Initialize the audio subsystem
    //audio_pool = init_audio();

    // Create FreeRTOS tasks for MIDI handling and LED blinking
    //xTaskCreate(usb_midi_task, "USB_MIDI_Task", 4096, NULL, configMAX_PRIORITIES, NULL);
    //xTaskCreate(blinker_task, "Blinker_Task", 128, NULL, configMAX_PRIORITIES - 1, NULL);

    // Start the FreeRTOS scheduler
    //vTaskStartScheduler();

    // Idle loop (this is fine for Cortex-M33)
    while (1) {
       // __wfi();
    }
}

void __not_in_flash_func(i2s_callback_func()) {
    audio_buffer_t *buffer = take_audio_buffer(audio_pool, false);
    if (buffer == NULL) {
        return;
    }

    int32_t *samples = (int32_t *)buffer->buffer->bytes;

    for (uint i = 0; i < buffer->max_sample_count; i++) {
        pd_prog.processInlineInterleaved(smp, smp, 1);
        samples[i * 2 + 0] = float_to_int32(smp[0]);
        samples[i * 2 + 1] = float_to_int32(smp[1]);
    }

    buffer->sample_count = buffer->max_sample_count;
    give_audio_buffer(audio_pool, buffer);
}

#ifdef __cplusplus
}
#endif