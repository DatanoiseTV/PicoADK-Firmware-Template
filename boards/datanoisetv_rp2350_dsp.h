// Pico-SDK board file for PicoADK v2 (RP2350A).
//
// Pico-SDK loads this when PICO_BOARD=datanoisetv_rp2350_dsp. It declares the
// onboard PSRAM size so pico-sdk's QMI bring-up reserves and memory-maps it,
// and pins the same default stdio/LED/SPI/I2C layout used by v1.
//
// Application-level pin assignments live in picoadk_v2.h (selected via
// PICOADK_BOARD_SELECT_V2) and are accessed through `<picoadk/hal/pins.h>`.

#ifndef _BOARDS_DATANOISETV_RP2350_DSP_H
#define _BOARDS_DATANOISETV_RP2350_DSP_H

#ifndef PICO_RP2350
#define PICO_RP2350 1
#endif
#ifndef PICO_RP2350A
#define PICO_RP2350A 1
#endif

// 8 MB QSPI PSRAM on CS1 — pico-sdk 2.x will probe and map this when present.
#ifndef PICO_RP2350_PSRAM_CS
#define PICO_RP2350_PSRAM_CS 1
#endif

// Default UART for stdio.
#ifndef PICO_DEFAULT_UART
#define PICO_DEFAULT_UART 0
#endif
#ifndef PICO_DEFAULT_UART_TX_PIN
#define PICO_DEFAULT_UART_TX_PIN 0
#endif
#ifndef PICO_DEFAULT_UART_RX_PIN
#define PICO_DEFAULT_UART_RX_PIN 1
#endif

#ifndef PICO_DEFAULT_LED_PIN
#define PICO_DEFAULT_LED_PIN 15
#endif

#ifndef PICO_DEFAULT_SPI
#define PICO_DEFAULT_SPI 1
#endif
#ifndef PICO_DEFAULT_SPI_SCK_PIN
#define PICO_DEFAULT_SPI_SCK_PIN 10
#endif
#ifndef PICO_DEFAULT_SPI_TX_PIN
#define PICO_DEFAULT_SPI_TX_PIN 11
#endif
#ifndef PICO_DEFAULT_SPI_RX_PIN
#define PICO_DEFAULT_SPI_RX_PIN 12
#endif
#ifndef PICO_DEFAULT_SPI_CSN_PIN
#define PICO_DEFAULT_SPI_CSN_PIN 13
#endif

#ifndef PICO_DEFAULT_I2C
#define PICO_DEFAULT_I2C 0
#endif
#ifndef PICO_DEFAULT_I2C_SDA_PIN
#define PICO_DEFAULT_I2C_SDA_PIN 24
#endif
#ifndef PICO_DEFAULT_I2C_SCL_PIN
#define PICO_DEFAULT_I2C_SCL_PIN 25
#endif

#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (4 * 1024 * 1024)
#endif

#endif
