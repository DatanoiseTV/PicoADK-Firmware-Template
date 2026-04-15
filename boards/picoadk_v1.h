// PicoADK v1 — RP2040 board pin map.
// Selected automatically when PICOADK_BOARD=v1_rp2040.

#pragma once

#define PICOADK_BOARD_NAME       "PicoADK v1 (RP2040)"
#define PICOADK_BOARD_V1         1
#define PICOADK_BOARD_V2         0
#define PICOADK_HAS_PSRAM        0
#define PICOADK_HAS_SDIO         0
#define PICOADK_HAS_HW_DIVIDER   1
#define PICOADK_HAS_FPU          0
#define PICOADK_HAS_DCP          0
#define PICOADK_DEFAULT_SYS_KHZ  402000
#define PICOADK_MAX_SYS_KHZ      420000

// I2S (DAC out, no input on v1 by default)
#define PICOADK_PIN_I2S_DOUT     16
#define PICOADK_PIN_I2S_BCK      17
#define PICOADK_PIN_I2S_WS       18
#define PICOADK_PIN_I2S_DIN      255   // not wired on v1

// DAC control
#define PICOADK_PIN_DAC_MUTE     25
#define PICOADK_PIN_DAC_DEEMPH   23

// MIDI UART
#define PICOADK_PIN_MIDI_UART_RX 5
#define PICOADK_PIN_MIDI_UART_TX 4
#define PICOADK_MIDI_UART_INST   uart1

// External 8-channel ADC (MCP3208)
#define PICOADK_PIN_ADC_SCK      10
#define PICOADK_PIN_ADC_TX       11
#define PICOADK_PIN_ADC_RX       12
#define PICOADK_PIN_ADC_CS       13
#define PICOADK_ADC_SPI_INST     spi1
#define PICOADK_ADC_SPI_HZ       16000000

// Encoder + button
#define PICOADK_PIN_ENC_BTN      19
#define PICOADK_PIN_ENC_A        20
#define PICOADK_PIN_ENC_B        21
#define PICOADK_PIN_TRIG_BTN     14

// OLED I2C
#define PICOADK_PIN_OLED_SDA     24
#define PICOADK_PIN_OLED_SCL     25
#define PICOADK_OLED_I2C_INST    i2c0

// Trigger / CV inputs (RP2040 ADC pins)
#define PICOADK_PIN_CV_IN_0      26
#define PICOADK_PIN_CV_IN_1      27
#define PICOADK_PIN_POT_0        28
#define PICOADK_PIN_POT_1        29

// Status LEDs (chase 2..5, note-on indicator 15)
#define PICOADK_PIN_LED_CHASE_0  2
#define PICOADK_PIN_LED_CHASE_1  3
#define PICOADK_PIN_LED_CHASE_2  4
#define PICOADK_PIN_LED_CHASE_3  5
#define PICOADK_PIN_LED_NOTEON   15

// SD card (SPI fallback only on v1)
#define PICOADK_SD_TRANSPORT_SPI 1
#define PICOADK_SD_TRANSPORT_PIO 0
