// Analog mux scanner — supports CD4051 / 74HC4067 chains.
//
// A FreeRTOS task walks each bank (set address pins → wait → sample →
// next channel) and stores the values in a double-buffered table. Reads are
// lock-free.

#include "picoadk/hal/mux.h"
#include "picoadk/hal/controls.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include <FreeRTOS.h>
#include <task.h>

#include <cstring>

namespace picoadk::Scanner {

namespace {

constexpr std::size_t kMaxInputs = 256;

ScannerConfig g_cfg{};
std::size_t   g_total = 0;
uint16_t      g_raw[kMaxInputs]{};
float         g_smoothed[kMaxInputs]{};
TaskHandle_t  g_task = nullptr;

uint16_t sample(AdcSource src, uint8_t ch) {
    return src == AdcSource::Mcp3208 ? Controls::read_adc8(ch) : Controls::read_adc(ch);
}

void scanner_task(void*) {
    for (;;) {
        std::size_t out_index = 0;
        for (std::size_t b = 0; b < g_cfg.num_banks; ++b) {
            const MuxBank& bank = g_cfg.banks[b];
            if (bank.enable_pin != 255) gpio_put(bank.enable_pin, 0);
            for (uint8_t ch = 0; ch < bank.num_channels; ++ch) {
                for (uint8_t a = 0; a < bank.addr_count; ++a) {
                    gpio_put(bank.addr_pins[a], (ch >> a) & 1);
                }
                if (bank.settle_us) busy_wait_us(bank.settle_us);
                uint16_t v = sample(bank.adc_source, bank.adc_channel);
                g_raw[out_index] = v;
                float n = v / 4095.0f;
                if (g_cfg.smoothing > 0)
                    g_smoothed[out_index] += (n - g_smoothed[out_index]) * g_cfg.smoothing;
                else
                    g_smoothed[out_index] = n;
                ++out_index;
            }
            if (bank.enable_pin != 255) gpio_put(bank.enable_pin, 1);
        }
        for (std::size_t d = 0; d < g_cfg.num_direct; ++d) {
            uint16_t v = sample(g_cfg.direct[d].adc_source, g_cfg.direct[d].adc_channel);
            g_raw[out_index] = v;
            float n = v / 4095.0f;
            if (g_cfg.smoothing > 0)
                g_smoothed[out_index] += (n - g_smoothed[out_index]) * g_cfg.smoothing;
            else
                g_smoothed[out_index] = n;
            ++out_index;
        }
        if (g_cfg.scan_rate_hz)
            vTaskDelay(pdMS_TO_TICKS(1000 / g_cfg.scan_rate_hz));
        else
            vTaskDelay(1);
    }
}

}  // namespace

std::size_t init(const ScannerConfig& cfg) {
    g_cfg = cfg;
    g_total = 0;
    for (std::size_t b = 0; b < cfg.num_banks; ++b) {
        const MuxBank& bank = cfg.banks[b];
        for (uint8_t a = 0; a < bank.addr_count; ++a) {
            gpio_init(bank.addr_pins[a]);
            gpio_set_dir(bank.addr_pins[a], GPIO_OUT);
            gpio_put(bank.addr_pins[a], 0);
        }
        if (bank.enable_pin != 255) {
            gpio_init(bank.enable_pin);
            gpio_set_dir(bank.enable_pin, GPIO_OUT);
            gpio_put(bank.enable_pin, 1);
        }
        g_total += bank.num_channels;
    }
    g_total += cfg.num_direct;
    if (g_total > kMaxInputs) g_total = kMaxInputs;

    if (!g_task) {
        xTaskCreate(scanner_task, "MUX", 1024, nullptr, tskIDLE_PRIORITY + 2, &g_task);
    }
    return g_total;
}

void stop() {
    if (g_task) { vTaskDelete(g_task); g_task = nullptr; }
}

uint16_t read(std::size_t i)         { return i < g_total ? g_raw[i] : 0; }
float    read_norm(std::size_t i)    { return i < g_total ? g_smoothed[i] : 0.0f; }
void     snapshot(uint16_t* dst, std::size_t max_len) {
    std::size_t n = max_len < g_total ? max_len : g_total;
    std::memcpy(dst, g_raw, n * sizeof(uint16_t));
}

}  // namespace picoadk::Scanner
