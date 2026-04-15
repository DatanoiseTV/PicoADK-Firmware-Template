// Controls HAL — MCP3208 SPI ADC + RP2040/RP2350 SAR ADC + status LEDs.

#include "picoadk/hal/controls.h"
#include "picoadk_board.h"

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/spi.h"

#include <initializer_list>

namespace picoadk::Controls {

namespace {
bool g_inited = false;

void init_status_leds() {
    for (uint8_t pin : {PICOADK_PIN_LED_CHASE_0, PICOADK_PIN_LED_CHASE_1,
                        PICOADK_PIN_LED_CHASE_2, PICOADK_PIN_LED_CHASE_3,
                        PICOADK_PIN_LED_NOTEON}) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, 0);
    }
}

void init_external_adc() {
    gpio_set_function(PICOADK_PIN_ADC_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PICOADK_PIN_ADC_TX,  GPIO_FUNC_SPI);
    gpio_set_function(PICOADK_PIN_ADC_RX,  GPIO_FUNC_SPI);
    gpio_init(PICOADK_PIN_ADC_CS);
    gpio_set_dir(PICOADK_PIN_ADC_CS, GPIO_OUT);
    gpio_put(PICOADK_PIN_ADC_CS, 1);
    spi_init(PICOADK_ADC_SPI_INST, PICOADK_ADC_SPI_HZ);
    spi_set_format(PICOADK_ADC_SPI_INST, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
}

void init_internal_adc() {
    adc_init();
    adc_gpio_init(PICOADK_PIN_CV_IN_0);
    adc_gpio_init(PICOADK_PIN_CV_IN_1);
    adc_gpio_init(PICOADK_PIN_POT_0);
    adc_gpio_init(PICOADK_PIN_POT_1);
}
}  // namespace

void init() {
    if (g_inited) return;
    g_inited = true;
    init_status_leds();
    init_external_adc();
    init_internal_adc();
}

uint16_t read_adc8(uint8_t channel) {
    if (channel > 7) return 0;
    gpio_put(PICOADK_PIN_ADC_CS, 0);
    // MCP3208 single-ended: start=1, sgl/diff=1, then 3-bit channel; then 4 idle bits
    uint8_t tx[3] = { uint8_t(0x06 | (channel >> 2)), uint8_t((channel & 0x03) << 6), 0 };
    uint8_t rx[3];
    spi_write_read_blocking(PICOADK_ADC_SPI_INST, tx, rx, 3);
    gpio_put(PICOADK_PIN_ADC_CS, 1);
    return ((uint16_t(rx[1]) & 0x0F) << 8) | rx[2];
}

uint16_t read_adc(uint8_t channel) {
    if (channel > 3) return 0;
    adc_select_input(channel);
    return adc_read();
}

float read_adc8_norm(uint8_t channel) { return read_adc8(channel) / 4095.0f; }
float read_adc_norm (uint8_t channel) { return read_adc (channel) / 4095.0f; }

EncoderState read_encoder() {
    // PHASE 1c: real implementation lives in src/hal/encoders.cpp.
    return EncoderState{};
}
void reset_encoder() {}

bool read_trig_button() {
    return !gpio_get(PICOADK_PIN_TRIG_BTN);
}

void set_status_leds(uint8_t mask) {
    gpio_put(PICOADK_PIN_LED_CHASE_0, (mask >> 0) & 1);
    gpio_put(PICOADK_PIN_LED_CHASE_1, (mask >> 1) & 1);
    gpio_put(PICOADK_PIN_LED_CHASE_2, (mask >> 2) & 1);
    gpio_put(PICOADK_PIN_LED_CHASE_3, (mask >> 3) & 1);
}

void set_note_led(bool on) { gpio_put(PICOADK_PIN_LED_NOTEON, on ? 1 : 0); }

}  // namespace picoadk::Controls
