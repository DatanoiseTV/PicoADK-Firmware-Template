// Pico-SDK board file for PicoADK v1 (RP2040).
//
// Pico-SDK loads this when PICO_BOARD=datanoisetv_rp2040_dsp. It overrides
// the default pin assignments for stdio UART, default LED, and similar bits.
//
// Application-level pin assignments live in picoadk_v1.h (selected via
// PICOADK_BOARD_SELECT_V1) and are accessed through `<picoadk/hal/pins.h>`.

#ifndef _BOARDS_DATANOISETV_RP2040_DSP_H
#define _BOARDS_DATANOISETV_RP2040_DSP_H

// Make sure the platform is right for this board.
#ifndef PICO_RP2040
#define PICO_RP2040 1
#endif

// Default UART for stdio (UART0 on the SWD header).
#ifndef PICO_DEFAULT_UART
#define PICO_DEFAULT_UART 0
#endif
#ifndef PICO_DEFAULT_UART_TX_PIN
#define PICO_DEFAULT_UART_TX_PIN 0
#endif
#ifndef PICO_DEFAULT_UART_RX_PIN
#define PICO_DEFAULT_UART_RX_PIN 1
#endif

// Default user LED.
#ifndef PICO_DEFAULT_LED_PIN
#define PICO_DEFAULT_LED_PIN 15
#endif

// SPI defaults (used by some pico_stdlib helpers — the firmware overrides
// these for the MCP3208 ADC bus.)
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

// I2C defaults (used by the OLED.)
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
