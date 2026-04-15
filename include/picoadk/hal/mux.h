// Analog mux scanner — for synths with way more pots than ADC channels.
//
// One MuxBank describes a CD4051 / 74HC4067 / chain feeding into a single
// "sample" pin (which itself can be a board ADC pin OR an MCP3208 channel).
// You can mix multiplexed and direct sources in one ScannerConfig and read
// them all with a single uniform `read(index)`.
//
// The scanner runs in the background (timer task) and double-buffers values
// so audio code can read latest values without locking.

#pragma once

#include <cstddef>
#include <cstdint>

namespace picoadk {

enum class AdcSource : uint8_t {
    BoardAdc,           // RP2040/RP2350 internal SAR ADC (channels 0..3)
    Mcp3208,            // External SPI ADC on the board
};

struct MuxBank {
    // Address pins (low to high). Up to 4 → 16:1 mux.
    uint8_t  addr_pins[4] = {255, 255, 255, 255};
    uint8_t  addr_count   = 0;             // 1..4
    uint8_t  num_channels = 0;             // 1..16

    // Where the mux output is sampled.
    AdcSource adc_source  = AdcSource::Mcp3208;
    uint8_t   adc_channel = 0;

    // Optional enable pin (active low). 255 = always enabled.
    uint8_t  enable_pin   = 255;

    // Settling time after switching address before sampling.
    uint16_t settle_us    = 8;
};

struct DirectInput {
    AdcSource adc_source  = AdcSource::Mcp3208;
    uint8_t   adc_channel = 0;
};

struct ScannerConfig {
    const MuxBank*     banks       = nullptr;
    std::size_t        num_banks   = 0;
    const DirectInput* direct      = nullptr;
    std::size_t        num_direct  = 0;
    uint16_t           scan_rate_hz = 1000;     // how often the whole bank is scanned
    float              smoothing   = 0.05f;     // 1-pole low-pass; 0 = no smoothing
};

namespace Scanner {

// Returns the total number of scannable inputs (sum of mux channels + direct).
std::size_t init(const ScannerConfig& cfg);

// Stop the background task.
void stop();

// 12-bit raw value, 0..4095.
uint16_t read(std::size_t index);

// Smoothed normalised value, 0.0..1.0.
float    read_norm(std::size_t index);

// Snapshot the entire scan into the caller's buffer (single contiguous read).
void     snapshot(uint16_t* dst, std::size_t max_len);

}  // namespace Scanner
}  // namespace picoadk
