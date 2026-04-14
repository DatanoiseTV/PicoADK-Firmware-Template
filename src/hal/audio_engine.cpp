// Audio engine — PIO + DMA + IRQ implementation.
//
// One TX state machine for output, one RX state machine for input (in
// duplex mode they share BCK/WS). Two ping-pong DMA channels per direction
// chase each other forever; each completion fires our IRQ which queues the
// "drained" buffer for the user callback to refill.
//
// The user callback runs from a high-priority FreeRTOS task — NOT from the
// IRQ — so its DSP work doesn't block PIO refills. The IRQ just signals a
// task notification. Latency is `block_size / sample_rate` per buffer.

#include "audio_engine.h"
#include "picoadk/dsp/math.h"
#include "picoadk_board.h"

#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/irq.h"

#include <FreeRTOS.h>
#include <task.h>

#include "i2s_stereo.pio.h"
#include "tdm.pio.h"

namespace picoadk::audio_engine {

namespace {

constexpr std::size_t kMaxBuffers  = 4;
constexpr std::size_t kMaxChannels = 8;
constexpr std::size_t kMaxFrames   = 256;

PIO              g_pio = pio0;
int              g_sm_out = -1;
int              g_sm_in  = -1;
int              g_dma_out_a = -1, g_dma_out_b = -1;
int              g_dma_in_a  = -1, g_dma_in_b  = -1;

// Ping-pong DMA buffers (interleaved int32) for each direction.
alignas(4) int32_t g_buf_out[2][kMaxFrames * kMaxChannels];
alignas(4) int32_t g_buf_in [2][kMaxFrames * kMaxChannels];

// Planar work buffers exposed to the user callback.
alignas(4) float   g_planar_in [kMaxChannels][kMaxFrames];
alignas(4) float   g_planar_out[kMaxChannels][kMaxFrames];

float*       g_in_ptrs [kMaxChannels];
float*       g_out_ptrs[kMaxChannels];
int32_t*     g_in_int_ptrs [kMaxChannels];
int32_t*     g_out_int_ptrs[kMaxChannels];

AudioConfig    g_cfg;
UserCallbacks  g_cb{};
TaskHandle_t   g_task = nullptr;

volatile uint8_t g_active_out = 0;     // index of the buffer DMA is currently sending
volatile uint8_t g_active_in  = 0;
volatile uint32_t g_load_x1000 = 0;
volatile bool     g_running = false;

// ---- IRQ handler — both DMA channels share one IRQ -----------------------
void __isr __not_in_flash_func(dma_irq_handler)() {
    BaseType_t hp_woken = pdFALSE;
    if (g_dma_out_a >= 0 && dma_channel_get_irq0_status(g_dma_out_a)) {
        dma_channel_acknowledge_irq0(g_dma_out_a);
        g_active_out = 0;
        if (g_task) vTaskNotifyGiveFromISR(g_task, &hp_woken);
    }
    if (g_dma_out_b >= 0 && dma_channel_get_irq0_status(g_dma_out_b)) {
        dma_channel_acknowledge_irq0(g_dma_out_b);
        g_active_out = 1;
        if (g_task) vTaskNotifyGiveFromISR(g_task, &hp_woken);
    }
    portYIELD_FROM_ISR(hp_woken);
}

// ---- Conversion helpers --------------------------------------------------
inline int32_t pad_to_int32(int32_t s, AudioBitDepth d) {
    switch (d) {
        case AudioBitDepth::Bits16: return s << 16;
        case AudioBitDepth::Bits24: return s << 8;
        default:                    return s;
    }
}
inline int32_t mask_from_int32(int32_t s, AudioBitDepth d) {
    switch (d) {
        case AudioBitDepth::Bits16: return s >> 16;
        case AudioBitDepth::Bits24: return s >> 8;
        default:                    return s;
    }
}

void deinterleave_in(uint8_t buf_idx, std::size_t frames, uint8_t channels) {
    const int32_t* src = g_buf_in[buf_idx];
    for (std::size_t f = 0; f < frames; ++f) {
        for (uint8_t c = 0; c < channels; ++c) {
            g_planar_in[c][f] = dsp::from_int32_sample(src[f * channels + c]);
            g_in_int_ptrs[c][f] = src[f * channels + c];
        }
    }
}
void interleave_out(uint8_t buf_idx, std::size_t frames, uint8_t channels) {
    int32_t* dst = g_buf_out[buf_idx];
    for (std::size_t f = 0; f < frames; ++f) {
        for (uint8_t c = 0; c < channels; ++c) {
            dst[f * channels + c] = dsp::to_int32_sample(g_planar_out[c][f]);
        }
    }
}

// ---- Worker task — runs the user callback off the IRQ --------------------
void __not_in_flash_func(audio_task)(void*) {
    const std::size_t frames   = g_cfg.block_size;
    const uint8_t     channels = g_cfg.num_channels;
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (!g_running) continue;

        // The just-completed buffer is the "other" one (1 - g_active_out).
        uint8_t fill_idx = 1 - g_active_out;

        absolute_time_t t0 = get_absolute_time();
        const bool has_in   = (static_cast<uint8_t>(g_cfg.direction) & static_cast<uint8_t>(AudioDirection::In)) != 0;
        if (has_in) deinterleave_in(fill_idx, frames, channels);

        if (g_cb.float_cb) {
            const float* const* in_ptr = has_in ? const_cast<const float* const*>(g_in_ptrs) : nullptr;
            g_cb.float_cb(in_ptr, g_out_ptrs, frames, g_cb.user);
        } else if (g_cb.int_cb) {
            const int32_t* const* in_ptr = has_in ? const_cast<const int32_t* const*>(g_in_int_ptrs) : nullptr;
            g_cb.int_cb(in_ptr, g_out_int_ptrs, frames, g_cb.user);
            for (std::size_t f = 0; f < frames; ++f)
                for (uint8_t c = 0; c < channels; ++c)
                    g_buf_out[fill_idx][f * channels + c] = g_out_int_ptrs[c][f];
        }
        if (g_cb.float_cb) interleave_out(fill_idx, frames, channels);

        // Bit-depth pad on the way out.
        if (g_cfg.bit_depth != AudioBitDepth::Bits32) {
            for (std::size_t i = 0; i < frames * channels; ++i) {
                g_buf_out[fill_idx][i] = pad_to_int32(g_buf_out[fill_idx][i] >> 0, g_cfg.bit_depth);
            }
        }

        absolute_time_t t1 = get_absolute_time();
        uint32_t budget_us = (frames * 1'000'000ull) / g_cfg.sample_rate_hz;
        uint32_t spent_us  = absolute_time_diff_us(t0, t1);
        g_load_x1000 = budget_us ? ((spent_us * 1000) / budget_us) : 0;
    }
}

// ---- Setup helpers -------------------------------------------------------
bool setup_pio_out(uint sample_rate, uint slots_per_frame) {
    g_sm_out = pio_claim_unused_sm(g_pio, true);
    if (g_sm_out < 0) return false;
    if (slots_per_frame <= 2) {
        uint off = pio_add_program(g_pio, &picoadk_i2s_out_program);
        picoadk_i2s_out_program_init(g_pio, g_sm_out, off,
                                     PICOADK_PIN_I2S_DOUT, PICOADK_PIN_I2S_BCK,
                                     sample_rate);
    } else {
        uint off = pio_add_program(g_pio, &picoadk_tdm_out_program);
        picoadk_tdm_out_program_init(g_pio, g_sm_out, off,
                                     PICOADK_PIN_I2S_DOUT, PICOADK_PIN_I2S_BCK,
                                     sample_rate, slots_per_frame);
    }
    return true;
}

bool setup_pio_in(uint sample_rate, uint slots_per_frame) {
#if PICOADK_PIN_I2S_DIN == 255
    return false;
#else
    g_sm_in = pio_claim_unused_sm(g_pio, true);
    if (g_sm_in < 0) return false;
    if (slots_per_frame <= 2) {
        uint off = pio_add_program(g_pio, &picoadk_i2s_in_program);
        picoadk_i2s_in_program_init(g_pio, g_sm_in, off,
                                    PICOADK_PIN_I2S_DIN, PICOADK_PIN_I2S_BCK,
                                    sample_rate);
    } else {
        uint off = pio_add_program(g_pio, &picoadk_tdm_in_program);
        picoadk_tdm_in_program_init(g_pio, g_sm_in, off,
                                    PICOADK_PIN_I2S_DIN, PICOADK_PIN_I2S_BCK,
                                    sample_rate, slots_per_frame);
    }
    return true;
#endif
}

void setup_dma_out(std::size_t bytes_per_buffer) {
    g_dma_out_a = dma_claim_unused_channel(true);
    g_dma_out_b = dma_claim_unused_channel(true);

    auto cfg = [&](int ch, int chain_to, void* src) {
        dma_channel_config c = dma_channel_get_default_config(ch);
        channel_config_set_transfer_data_size(&c, DMA_SIZE_32);
        channel_config_set_read_increment(&c, true);
        channel_config_set_write_increment(&c, false);
        channel_config_set_dreq(&c, pio_get_dreq(g_pio, g_sm_out, true));
        channel_config_set_chain_to(&c, chain_to);
        dma_channel_configure(ch, &c,
                              &g_pio->txf[g_sm_out], src,
                              bytes_per_buffer / 4, false);
        dma_channel_set_irq0_enabled(ch, true);
    };
    cfg(g_dma_out_a, g_dma_out_b, g_buf_out[0]);
    cfg(g_dma_out_b, g_dma_out_a, g_buf_out[1]);
}

void setup_dma_in(std::size_t bytes_per_buffer) {
    if (g_sm_in < 0) return;
    g_dma_in_a = dma_claim_unused_channel(true);
    g_dma_in_b = dma_claim_unused_channel(true);

    auto cfg = [&](int ch, int chain_to, void* dst) {
        dma_channel_config c = dma_channel_get_default_config(ch);
        channel_config_set_transfer_data_size(&c, DMA_SIZE_32);
        channel_config_set_read_increment(&c, false);
        channel_config_set_write_increment(&c, true);
        channel_config_set_dreq(&c, pio_get_dreq(g_pio, g_sm_in, false));
        channel_config_set_chain_to(&c, chain_to);
        dma_channel_configure(ch, &c, dst,
                              &g_pio->rxf[g_sm_in],
                              bytes_per_buffer / 4, false);
    };
    cfg(g_dma_in_a, g_dma_in_b, g_buf_in[0]);
    cfg(g_dma_in_b, g_dma_in_a, g_buf_in[1]);
}

}  // namespace

bool setup(const AudioConfig& cfg) {
    if (cfg.block_size > kMaxFrames || cfg.num_channels > kMaxChannels) return false;
    g_cfg = cfg;
    for (uint8_t c = 0; c < kMaxChannels; ++c) {
        g_in_ptrs [c]     = g_planar_in [c];
        g_out_ptrs[c]     = g_planar_out[c];
    }

    const uint8_t  slots = cfg.slots_per_frame;
    const std::size_t bytes_per_buffer = cfg.block_size * cfg.num_channels * 4;
    const bool need_out = (static_cast<uint8_t>(cfg.direction) & static_cast<uint8_t>(AudioDirection::Out)) != 0;
    const bool need_in  = (static_cast<uint8_t>(cfg.direction) & static_cast<uint8_t>(AudioDirection::In))  != 0;

    if (need_out && !setup_pio_out(cfg.sample_rate_hz, slots)) return false;
    if (need_in  && !setup_pio_in (cfg.sample_rate_hz, slots)) return false;

    if (need_out) setup_dma_out(bytes_per_buffer);
    if (need_in)  setup_dma_in (bytes_per_buffer);

    irq_set_exclusive_handler(DMA_IRQ_0, dma_irq_handler);
    irq_set_priority(DMA_IRQ_0, 0x40);
    irq_set_enabled(DMA_IRQ_0, true);

    if (!g_task) {
        xTaskCreate(audio_task, "AUDIO", 2048, nullptr, configMAX_PRIORITIES - 1, &g_task);
    }
    return true;
}

void teardown() {
    g_running = false;
    if (g_dma_out_a >= 0) { dma_channel_unclaim(g_dma_out_a); g_dma_out_a = -1; }
    if (g_dma_out_b >= 0) { dma_channel_unclaim(g_dma_out_b); g_dma_out_b = -1; }
    if (g_dma_in_a  >= 0) { dma_channel_unclaim(g_dma_in_a);  g_dma_in_a  = -1; }
    if (g_dma_in_b  >= 0) { dma_channel_unclaim(g_dma_in_b);  g_dma_in_b  = -1; }
    if (g_sm_out    >= 0) { pio_sm_unclaim(g_pio, g_sm_out);  g_sm_out    = -1; }
    if (g_sm_in     >= 0) { pio_sm_unclaim(g_pio, g_sm_in);   g_sm_in     = -1; }
}

void start() {
    g_running = true;
    if (g_dma_out_a >= 0) dma_channel_start(g_dma_out_a);
    if (g_dma_in_a  >= 0) dma_channel_start(g_dma_in_a);
}
void stop()       { g_running = false; }
bool running()    { return g_running; }
float measured_load() { return g_load_x1000 * 0.001f; }
void set_callbacks(const UserCallbacks& cb) { g_cb = cb; }

}  // namespace picoadk::audio_engine
