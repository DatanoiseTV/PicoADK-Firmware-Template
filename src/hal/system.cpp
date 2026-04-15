// System HAL — clocks, voltage, watchdog, stdio routing, FreeRTOS hooks.
//
// Stdio routing strategy:
//   * UART is the default debug surface (`stdio_uart_init`). Wired through
//     pico-sdk's stdio_driver so `printf` / `puts` Just Work.
//   * USB-CDC is opt-in: the user enables `UsbClass::Cdc` in `Usb::configure()`
//     before calling `Hardware::init()`. We then add a stdio_driver that
//     pumps to TinyUSB's CDC endpoints. Both can be active at the same time;
//     pico-sdk multiplexes.
//
// FreeRTOS hooks live here too (single home for kernel callbacks).

#include "picoadk/hal/system.h"
#include "picoadk/hal/usb_device.h"
#include "picoadk_board.h"

#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/stdio.h"
#include "pico/stdio/driver.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "hardware/watchdog.h"

#include <FreeRTOS.h>
#include <task.h>

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

// ---- Overclock voltage ladder -----------------------------------------------
//
// Pattern borrowed from arduino-pico's RP2040Support: pick the lowest vreg
// step that's comfortable at a given target frequency. Walk the table in
// order, stop at the first entry whose `max_khz` is ≥ target.
//
// Values err on the conservative side; silicon lottery may allow more.
struct VoltageStep {
    uint32_t      max_khz;
    enum vreg_voltage voltage;
};

#if PICOADK_BOARD_V1
// RP2040 — empirically the community runs these boards at 400 MHz daily.
constexpr VoltageStep kVoltageLadder[] = {
    { 133000, VREG_VOLTAGE_1_10 },    // stock
    { 225000, VREG_VOLTAGE_1_15 },
    { 275000, VREG_VOLTAGE_1_20 },
    { 350000, VREG_VOLTAGE_1_25 },
    { 420000, VREG_VOLTAGE_1_30 },    // max
};
#else
// RP2350 — Raspberry Pi publishes 150 MHz as the stock rate. Community
// testing shows stable operation up to ~300 MHz at 1.25 V with PSRAM;
// without PSRAM some boards push higher, but the QMI timing gets tight.
constexpr VoltageStep kVoltageLadder[] = {
    { 150000, VREG_VOLTAGE_1_10 },    // stock
    { 225000, VREG_VOLTAGE_1_15 },
    { 275000, VREG_VOLTAGE_1_20 },
    { 300000, VREG_VOLTAGE_1_25 },    // max (w/ PSRAM)
};
#endif

enum vreg_voltage voltage_for(uint32_t khz) {
    for (const auto& s : kVoltageLadder)
        if (khz <= s.max_khz) return s.voltage;
    return kVoltageLadder[sizeof(kVoltageLadder) / sizeof(kVoltageLadder[0]) - 1].voltage;
}

constexpr std::size_t kMaxClockChangedCallbacks = 4;
ClockChangedCallback g_clock_cbs[kMaxClockChangedCallbacks] = {};

// ---- USB-CDC stdio driver ----------------------------------------------
// Routes pico-sdk's stdio_driver writes through TinyUSB CDC. Reads are
// best-effort (CDC RX is small); writes auto-flush. Multiple stdio drivers
// can be enabled concurrently — anything you printf goes to all of them.

void cdc_out_chars(const char* buf, int length) {
    if (!tud_cdc_connected()) return;
    uint32_t written = 0;
    while (written < (uint32_t)length) {
        uint32_t avail = tud_cdc_write_available();
        if (!avail) { tud_cdc_write_flush(); continue; }
        uint32_t n = (avail < (uint32_t)length - written) ? avail : (uint32_t)length - written;
        tud_cdc_write(buf + written, n);
        written += n;
    }
    tud_cdc_write_flush();
}
int cdc_in_chars(char* buf, int length) {
    if (!tud_cdc_connected() || !tud_cdc_available()) return PICO_ERROR_NO_DATA;
    uint32_t n = tud_cdc_read(buf, (uint32_t)length);
    return n ? (int)n : PICO_ERROR_NO_DATA;
}
stdio_driver_t g_cdc_driver = {
    .out_chars       = cdc_out_chars,
    .out_flush       = nullptr,
    .in_chars        = cdc_in_chars,
    .next            = nullptr,
#if PICO_STDIO_ENABLE_CRLF_SUPPORT
    .last_ended_with_cr = false,
    .crlf_enabled    = true,
#endif
};
}  // namespace

void init(const SystemConfig& cfg) {
    if (g_inited) return;
    g_inited = true;

    uint32_t khz = cfg.sys_clock_khz ? cfg.sys_clock_khz : PICOADK_DEFAULT_SYS_KHZ;
    if (khz > max_safe_clock_khz()) khz = max_safe_clock_khz();

    // Bring core voltage up BEFORE raising the clock.
    vreg_set_voltage(voltage_for(khz));
    sleep_ms(2);
    set_sys_clock_khz(khz, true);
    g_sys_khz = khz;

    board_init();
    tusb_init();

    if (cfg.enable_stdio_uart) {
        stdio_uart_init();
    }

    // CDC stdio is automatic when the USB descriptor advertised CDC AND the
    // app asked for it via `UsbConfig::cdc_routes_stdio`.
    if (any(Usb::config().classes, UsbClass::Cdc) && Usb::config().cdc_routes_stdio) {
        stdio_set_driver_enabled(&g_cdc_driver, true);
    }

    if (cfg.enable_watchdog) {
        watchdog_enable(cfg.watchdog_timeout_ms, true);
    }
}

void feed_watchdog()       { watchdog_update(); }
uint64_t micros()          { return to_us_since_boot(get_absolute_time()); }
uint32_t millis()          { return to_ms_since_boot(get_absolute_time()); }
uint32_t sys_clock_khz()   { return g_sys_khz; }
const char* board_name()   { return PICOADK_BOARD_NAME; }

uint32_t max_safe_clock_khz() {
    return kVoltageLadder[sizeof(kVoltageLadder) / sizeof(kVoltageLadder[0]) - 1].max_khz;
}

bool set_clock_khz(uint32_t khz) {
    if (khz > max_safe_clock_khz()) return false;

    // If we're going DOWN, clock first then drop voltage.
    // If we're going UP, raise voltage first, wait, then clock.
    const bool going_up = khz > g_sys_khz;
    const enum vreg_voltage target_v = voltage_for(khz);

    if (going_up) {
        vreg_set_voltage(target_v);
        sleep_ms(2);
    }
    if (!set_sys_clock_khz(khz, false)) return false;
    g_sys_khz = khz;
    if (!going_up) {
        vreg_set_voltage(target_v);
        // No post-wait needed when dropping voltage.
    }

    // Tell PSRAM / I²S / anything clock-bound to retune.
    for (auto cb : g_clock_cbs) if (cb) cb(khz);
    return true;
}

void on_clock_changed(ClockChangedCallback cb) {
    for (auto& slot : g_clock_cbs) {
        if (!slot) { slot = cb; return; }
    }
    // Silent drop if we run out of slots; at most `kMaxClockChangedCallbacks`
    // subsystems can register. Bump the cap if we need more.
}

}  // namespace picoadk::System

// ---- ESP-IDF-style core-affinity task wrapper ------------------------------
namespace picoadk::Tasks {

bool create_pinned(void (*entry)(void*), const char* name, uint32_t stack_words,
                   void* arg, unsigned priority, void** out_handle, int core_id) {
    TaskHandle_t h = nullptr;
    if (xTaskCreate(entry, name, stack_words, arg, (UBaseType_t)priority, &h) != pdPASS)
        return false;
#if (configNUMBER_OF_CORES > 1) && (configUSE_CORE_AFFINITY == 1)
    if (core_id >= 0 && core_id < (int)configNUMBER_OF_CORES) {
        vTaskCoreAffinitySet(h, (UBaseType_t)(1u << core_id));
    }
#else
    (void)core_id;
#endif
    if (out_handle) *out_handle = h;
    return true;
}

}  // namespace picoadk::Tasks

// ---- FreeRTOS hooks --------------------------------------------------------
extern "C" void vApplicationStackOverflowHook(TaskHandle_t /*task*/, char* /*name*/) {
    __asm volatile("bkpt #0");
    for (;;) {}
}
extern "C" void vApplicationMallocFailedHook(void) {
    __asm volatile("bkpt #0");
    for (;;) {}
}
