// Sample player — streams from SD, with pitch shift, looping, multisampling.
//
// Two backends:
//   * MemorySampleSource — entire WAV/AIFF in RAM/PSRAM. Best latency.
//   * StreamingSampleSource — file read on demand, double-buffered ring.
//                             Suitable for long samples that don't fit RAM.
//
// Voices are pooled so multiple notes can fire from the same sample without
// re-reading the file. A KeyMap lets you assign different samples to MIDI
// note ranges (multisamples).
//
// Phase 7 fills this in. The header reserves the public API now so apps can
// be written against it.

#pragma once

#include <cstddef>
#include <cstdint>

#include "picoadk/dsp/real.h"

namespace picoadk::dsp {

class SampleSource {
public:
    virtual ~SampleSource() = default;
    virtual std::size_t channels() const = 0;
    virtual std::size_t length()   const = 0;     // frames
    virtual std::size_t sample_rate_hz() const = 0;

    // Read `frames` frames at planar `pos`. Returns frames actually returned.
    virtual std::size_t read(std::size_t pos, Real** out, std::size_t frames) = 0;
};

class MemorySampleSource final : public SampleSource {
public:
    static MemorySampleSource* load_wav_from_sd(const char* path);
    static MemorySampleSource* load_wav_from_memory(const uint8_t* data, std::size_t bytes);
    ~MemorySampleSource() override;

    std::size_t channels()      const override { return ch_; }
    std::size_t length()        const override { return len_; }
    std::size_t sample_rate_hz()const override { return sr_; }
    std::size_t read(std::size_t pos, Real** out, std::size_t frames) override;
private:
    Real**      data_ = nullptr;
    std::size_t ch_ = 0, len_ = 0, sr_ = 0;
};

class StreamingSampleSource final : public SampleSource {
public:
    static StreamingSampleSource* open_wav(const char* path,
                                           std::size_t prefetch_frames = 8192);
    ~StreamingSampleSource() override;

    std::size_t channels()       const override;
    std::size_t length()         const override;
    std::size_t sample_rate_hz() const override;
    std::size_t read(std::size_t pos, Real** out, std::size_t frames) override;
private:
    void* impl_ = nullptr;     // opaque StreamImpl*
};

enum class SamplerInterp : uint8_t { Nearest, Linear, Cubic };

// Single voice: plays one source at a configurable speed/pitch.
class SamplePlayer {
public:
    void  reset(float engine_sample_rate);
    void  set_source(SampleSource* src);
    void  set_speed(float speed);          // 1.0 = native
    void  set_pitch_semitones(float semis);// alternative to set_speed
    void  set_loop(bool loop, std::size_t loop_start = 0, std::size_t loop_end = 0);
    void  set_interp(SamplerInterp i);
    void  set_time_stretch(bool on, float stretch_ratio = 1.0f); // grain-based, ratio independent of pitch
    void  trigger();                        // (re)start from start
    void  stop();
    bool  active() const noexcept;

    // Process into stereo output, mixing on top.
    void  process(Real* out_l, Real* out_r, std::size_t frames);
private:
    SampleSource* src_ = nullptr;
    float sr_ = 48000.0f, speed_ = 1.0f, pos_ = 0.0f;
    bool  active_ = false, loop_ = false, stretch_ = false;
    std::size_t loop_a_ = 0, loop_b_ = 0;
    SamplerInterp interp_ = SamplerInterp::Linear;
    float stretch_ratio_ = 1.0f;
};

// Multisampled keyboard — picks the right sample for the incoming MIDI note
// and pitches it the residual amount.
struct KeyZone {
    SampleSource* source;
    uint8_t       lo_note;
    uint8_t       hi_note;
    uint8_t       root_note;
    float         gain = 1.0f;
};

class MultisamplePlayer {
public:
    static constexpr std::size_t kMaxVoices = 16;

    void   reset(float engine_sample_rate, std::size_t voice_count);
    void   set_zones(const KeyZone* zones, std::size_t count);
    void   note_on (uint8_t note, uint8_t velocity);
    void   note_off(uint8_t note);
    void   process (Real* out_l, Real* out_r, std::size_t frames);

    std::size_t voice_count()  const noexcept { return voice_count_; }
    std::size_t voices_active() const noexcept;

private:
    struct Voice {
        SamplePlayer player;
        uint8_t      note    = 0;
        bool         active  = false;
        uint32_t     age     = 0;
        float        gain    = 1.0f;
    };
    Voice          voices_[kMaxVoices];
    std::size_t    voice_count_ = 0;
    const KeyZone* zones_       = nullptr;
    std::size_t    zone_count_  = 0;
    float          sr_          = 48000.0f;
    uint32_t       age_counter_ = 0;
};

}  // namespace picoadk::dsp
