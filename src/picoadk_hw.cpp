#include "picoadk_hw.h"
#include "hardware/structs/xip_ctrl.h"

void picoadk_init()
{
        vreg_set_voltage(VREG_VOLTAGE_1_30);
        sleep_ms(1);
        set_sys_clock_khz(240000, true);

        // Initialize TinyUSB
        board_init();

        // tusb_init(0, TUSB_ROLE_DEVICE);
        #if TUSB_VERSION_NUMBER >= 1700
            tusb_init(0, TUSB_ROLE_DEVICE);
        #else
            tusb_init();
        #endif

        stdio_init_all();

        // set gpio 25 (soft mute) to output and set to 1 (unmute)
        gpio_init(25);
        gpio_set_dir(25, GPIO_OUT);
        gpio_put(25, 1);

        // set gpio 23 (deemphasis) to output and set to 1 (enable)
        gpio_init(23);
        gpio_set_dir(23, GPIO_OUT);
        gpio_put(23, 1);

        // LED on GPIO15
        gpio_init(15);
        gpio_set_dir(15, GPIO_OUT);

        adc_init();

        // Make sure GPIO is high-impedance, no pullups etc
        adc_gpio_init(26);
        // Select ADC input 0 (GPIO26)
        adc_select_input(0);

        srand(adc_read());

        uint32_t rand_seed;
        for (int i = 0; i < 32; i++)
        {
                bool randomBit = rosc_hw->randombit;
                rand_seed = rand_seed | (randomBit << i);
        }

        srand(rand_seed);
}

int adc128_read(uint8_t chan)
{
        if (chan > 7)
                return 0;
        gpio_put(13, 0);
        uint8_t data[2] = {(uint8_t)(chan << 3), 0};
        uint8_t rxData[2];
        spi_write_read_blocking(spi1, data, rxData, 2);
        gpio_put(13, 1);
        uint16_t result = (rxData[0] << 8) | rxData[1];

        return result;
}


#define PSRAM_BASE_ADDRESS ((volatile uint8_t*)0x11000000)
extern "C" float psram_read(uint32_t addr)
{
        // convert the 32-bit result from the psram to float
        uint32_t result = *((uint32_t*)(PSRAM_BASE_ADDRESS + addr));
        return (float)result / 0x7FFFFFFF;
}

extern "C" int32_t psram_write(uint32_t addr, float value)
{
        // convert float (-1.0 to 1.0) to 32-bit integer (0x80000000 to 0x7FFFFFFF)
        uint32_t data = (uint32_t)(value * 0x7FFFFFFF);
        // write the 32-bit integer to the psram
        *((uint32_t*)(PSRAM_BASE_ADDRESS + addr)) = data;
        return 0;
}
