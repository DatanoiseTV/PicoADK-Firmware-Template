// Audio HAL.
//
// One callback gets called every audio block. The signature is the same in
// all directions — when there is no input the `in` pointer is nullptr.
//
//     void my_cb(const float* const* in, float* const* out,
//                size_t frames, void* user);
//
// `in` and `out` are arrays of channel pointers (de-interleaved); each
// channel is `frames` floats in [-1.0, 1.0]. Float-in / float-out keeps the
// user code identical between RP2040 (where the HAL converts to/from Q16
// internally) and RP2350 (native float). Use AudioCallback if you want to
// receive raw int32 samples instead.

#pragma once

#include <cstddef>
#include <cstdint>
#include <stddef.h>     // belt-and-braces for non-libcxx hosts

namespace picoadk {

enum class AudioDirection : uint8_t {
    Out = 0b01,
    In  = 0b10,
    Duplex = Out | In,
};

enum class AudioBitDepth : uint8_t {
    Bits16 = 16,
    Bits24 = 24,
    Bits32 = 32,
};

enum class AudioFormat : uint8_t {
    FloatInterleaved = 0,    // float[frames * channels] — easiest for FX
    FloatPlanar      = 1,    // float* per channel       — easiest for DSP
    Int32Planar      = 2,    // raw I2S samples, no conversion
};

// Frame format on the wire. The PIO state machines are parameterised by
// `slots_per_frame` and `slot_width` so codecs / ADCs / DACs that don't
// speak vanilla 2-slot I2S still work.
enum class AudioFrameFormat : uint8_t {
    I2S          = 0,    // Philips I2S (data shifted 1 BCK after WS edge)
    LeftJustified,       // No 1-BCK delay; MSB on the WS edge
    RightJustified,
    TDM,                 // multi-slot frame, slots_per_frame > 2
};

enum class AudioPolarity : uint8_t { Normal, Inverted };

struct AudioConfig {
    uint32_t          sample_rate_hz   = 48000;
    AudioBitDepth     bit_depth        = AudioBitDepth::Bits32;
    uint16_t          block_size       = 32;     // frames per callback
    uint8_t           num_channels     = 2;      // user-visible channels (≤ slots_per_frame)
    AudioDirection    direction        = AudioDirection::Out;
    AudioFormat       format           = AudioFormat::FloatPlanar;
    uint8_t           num_buffers      = 3;      // ring depth

    // Wire format. Defaults give vanilla stereo I2S.
    AudioFrameFormat  frame_format     = AudioFrameFormat::I2S;
    uint8_t           slots_per_frame  = 2;      // 2 = stereo I2S; 4/8 = TDM
    uint8_t           slot_width       = 32;     // bits per slot on the wire
    AudioPolarity     bclk_polarity    = AudioPolarity::Normal;
    AudioPolarity     ws_polarity      = AudioPolarity::Normal;
};

// Float planar callback (default). `in` may be nullptr when direction == Out.
using AudioFloatCallback = void (*)(const float* const* in,
                                    float* const* out,
                                    std::size_t frames,
                                    void* user);

// Int32 planar callback for users who want raw I2S samples.
using AudioInt32Callback = void (*)(const int32_t* const* in,
                                    int32_t* const* out,
                                    std::size_t frames,
                                    void* user);

namespace Audio {

bool init(const AudioConfig& cfg);
void start();
void stop();
bool is_running();

void set_callback(AudioFloatCallback cb, void* user = nullptr);
void set_callback(AudioInt32Callback cb, void* user = nullptr);

// Live reconfiguration. The audio engine is stopped, reinitialised with the
// new settings, and started again. Any in-flight buffers are dropped.
bool reconfigure(const AudioConfig& cfg);

const AudioConfig& config();

// Most recent measured DSP load (0.0 .. 1.0+; >1.0 means we missed deadlines).
float load();

}  // namespace Audio
}  // namespace picoadk
