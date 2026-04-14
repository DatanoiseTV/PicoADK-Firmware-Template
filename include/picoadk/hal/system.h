// System-level helpers: clocks, voltage, watchdog, reset reasons.

#pragma once

#include <cstdint>

namespace picoadk {

struct SystemConfig {
    uint32_t sys_clock_khz = 0;     // 0 = board default
    bool     enable_watchdog = true;
    uint32_t watchdog_timeout_ms = 8000;
    bool     enable_stdio_uart = true;
    bool     enable_stdio_usb  = false;  // collides with TinyUSB device mode
};

namespace System {

// Apply clock + voltage + stdio + watchdog. Safe to call once at boot.
// If sys_clock_khz is 0 the board default is used; values above the per-board
// PICOADK_MAX_SYS_KHZ are clamped down with a warning.
void init(const SystemConfig& cfg = {});

// Pat the watchdog. Call from a task that runs at least every few seconds.
void feed_watchdog();

// Microsecond timestamp since boot.
uint64_t micros();

// Millisecond timestamp since boot.
uint32_t millis();

// Current system clock, kHz.
uint32_t sys_clock_khz();

// Board-friendly name string ("PicoADK v1 (RP2040)" / "PicoADK v2 (RP2350)").
const char* board_name();

}  // namespace System
}  // namespace picoadk
