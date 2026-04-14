// System HAL implementation — clocks, watchdog, stdio, board ID.

#include "picoadk/hal/system.h"
#include "picoadk_board.h"

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "hardware/watchdog.h"

#if __has_include("bsp/board_api.h")
#  include "bsp/board_api.h"
#else
#  include "bsp/board.h"
#endif
#include "tusb.h"

namespace picoadk::System {

namespace {
uint32_t g_sys_khz = PICOADK_DEFAULT_SYS_KHZ;
bool     g_inited  = false;
}

void init(const SystemConfig& cfg) {
    if (g_inited) return;
    g_inited = true;

    uint32_t khz = cfg.sys_clock_khz ? cfg.sys_clock_khz : PICOADK_DEFAULT_SYS_KHZ;
    if (khz > PICOADK_MAX_SYS_KHZ) khz = PICOADK_MAX_SYS_KHZ;

#if PICOADK_BOARD_V1
    // RP2040 needs higher core voltage to clock past ~250 MHz reliably.
    if (khz > 250'000) {
        vreg_set_voltage(VREG_VOLTAGE_1_30);
        sleep_ms(1);
    }
#endif
    set_sys_clock_khz(khz, true);
    g_sys_khz = khz;

    board_init();
    tusb_init();
    if (cfg.enable_stdio_uart) stdio_uart_init();
    if (cfg.enable_stdio_usb)  stdio_usb_init();

    if (cfg.enable_watchdog) {
        watchdog_enable(cfg.watchdog_timeout_ms, true);
    }
}

void feed_watchdog()         { watchdog_update(); }
uint64_t micros()            { return to_us_since_boot(get_absolute_time()); }
uint32_t millis()            { return to_ms_since_boot(get_absolute_time()); }
uint32_t sys_clock_khz()     { return g_sys_khz; }
const char* board_name()     { return PICOADK_BOARD_NAME; }

}  // namespace picoadk::System
