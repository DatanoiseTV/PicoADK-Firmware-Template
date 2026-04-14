// PicoADK v2 — RP2350 board pin map.
// Selected automatically when PICOADK_BOARD=v2_rp2350.
//
// Pin assignments default to v1 layout where the function is the same;
// override here as the v2 schematic settles. Anything tagged TODO(v2) needs
// schematic confirmation before production use.

#pragma once

#define PICOADK_BOARD_NAME       "PicoADK v2 (RP2350)"
#define PICOADK_BOARD_V1         0
#define PICOADK_BOARD_V2         1
#define PICOADK_HAS_PSRAM        1
#define PICOADK_HAS_SDIO         1
#define PICOADK_HAS_HW_DIVIDER   0   // RP2350 dropped the SIO divider; use M33 SDIV/UDIV
#define PICOADK_HAS_FPU          1   // Cortex-M33 single-precision FPU
#define PICOADK_HAS_DCP          1   // Double-precision coprocessor
#define PICOADK_DEFAULT_SYS_KHZ  150000
#define PICOADK_MAX_SYS_KHZ      300000

// I2S — full duplex capable
#define PICOADK_PIN_I2S_DOUT     16
#define PICOADK_PIN_I2S_BCK      17
#define PICOADK_PIN_I2S_WS       18
#define PICOADK_PIN_I2S_DIN      19   // TODO(v2): confirm

// DAC control
#define PICOADK_PIN_DAC_MUTE     22
#define PICOADK_PIN_DAC_DEEMPH   23

// MIDI UART
#define PICOADK_PIN_MIDI_UART_RX 5
#define PICOADK_PIN_MIDI_UART_TX 4
#define PICOADK_MIDI_UART_INST   uart1

// External ADC (MCP3208) — same SPI pinout
#define PICOADK_PIN_ADC_SCK      10
#define PICOADK_PIN_ADC_TX       11
#define PICOADK_PIN_ADC_RX       12
#define PICOADK_PIN_ADC_CS       13
#define PICOADK_ADC_SPI_INST     spi1
#define PICOADK_ADC_SPI_HZ       20000000

// Encoder + button
#define PICOADK_PIN_ENC_BTN      6
#define PICOADK_PIN_ENC_A        7
#define PICOADK_PIN_ENC_B        8
#define PICOADK_PIN_TRIG_BTN     14

// OLED I2C
#define PICOADK_PIN_OLED_SDA     24
#define PICOADK_PIN_OLED_SCL     25
#define PICOADK_OLED_I2C_INST    i2c0

// CV / pots
#define PICOADK_PIN_CV_IN_0      26
#define PICOADK_PIN_CV_IN_1      27
#define PICOADK_PIN_POT_0        28
#define PICOADK_PIN_POT_1        29

// Status LEDs
#define PICOADK_PIN_LED_CHASE_0  2
#define PICOADK_PIN_LED_CHASE_1  3
#define PICOADK_PIN_LED_CHASE_2  4
#define PICOADK_PIN_LED_CHASE_3  5
#define PICOADK_PIN_LED_NOTEON   15

// SDIO 4-bit (PIO driven). TODO(v2): confirm card-detect line.
#define PICOADK_PIN_SDIO_CLK     34
#define PICOADK_PIN_SDIO_CMD     35
#define PICOADK_PIN_SDIO_D0      36
#define PICOADK_PIN_SDIO_D1      37
#define PICOADK_PIN_SDIO_D2      38
#define PICOADK_PIN_SDIO_D3      39
#define PICOADK_PIN_SD_CD        40

#define PICOADK_SD_TRANSPORT_SPI 0
#define PICOADK_SD_TRANSPORT_PIO 1

// PSRAM (RP2350 QMI-attached). Linker reserves .psram_bss / .psram_data.
#define PICOADK_PSRAM_SIZE_BYTES (8u * 1024u * 1024u)
