#include "picoadk_hw.h"

void picoadk_init(){
        // Overclock the CPU to 226 MHz.
        // We choose 226 MHz, because it works the best for the audio subsystem.
        vreg_set_voltage(VREG_VOLTAGE_1_30);
        sleep_ms(1);
        set_sys_clock_khz(226000, true);

        // Initialize TinyUSB
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

        // LED on GPIO15
        gpio_init(15);
        gpio_set_dir(15, GPIO_OUT);
}