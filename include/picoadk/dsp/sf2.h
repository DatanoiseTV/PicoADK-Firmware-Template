// SoundFont 2 (.sf2) and SFZ (.sfz) playback.
//
// Sf2Player wraps TinySoundFont (single-header, BSD-2 / public domain).
// Loads a soundfont from memory or SD, exposes a MIDI-driven render that
// fits the picoadk audio callback. Polyphony is configurable.
//
// SfzPlayer is a placeholder skeleton — full SFZ ingestion is a serious
// undertaking. For 99 % of use cases the SF2 path covers the same ground
// (the GS/GM banks ship as SF2 everywhere). The SFZ stub exists so apps can
// be written against the API.

#pragma once

#include <cstddef>
#include <cstdint>

#include "picoadk/dsp/sample_player.h"     // KeyZone / MultisamplePlayer

namespace picoadk::dsp {

class Sf2Player {
public:
    Sf2Player();
    ~Sf2Player();

    // Load from memory (e.g. a soundfont compiled into flash, or PSRAM-resident
    // bytes). The buffer must outlive the player.
    bool load_from_memory(const void* data, std::size_t bytes);

    // Load from an SD path. Requires Storage to be mounted; the file is read
    // fully into RAM/PSRAM (TinySoundFont needs random access).
    bool load_from_sd(const char* path);

    void close();

    bool loaded() const noexcept;
    int  preset_count() const noexcept;

    // Pick a preset. Negative bank/preset ⇒ first preset.
    void set_preset(int bank, int preset_number);

    // Engine sample rate. Must match the audio HAL's sample_rate_hz.
    void set_sample_rate(float sr);

    // Master gain (linear).
    void set_gain(float linear);

    // MIDI events.
    void note_on (int channel, int note, float velocity);
    void note_off(int channel, int note);
    void all_notes_off(int channel);
    void program_change(int channel, int preset);
    void pitch_bend(int channel, float semitones);
    void cc(int channel, int controller, int value);

    // Render `frames` of stereo float audio (additive into out_l/out_r).
    void process(float* out_l, float* out_r, std::size_t frames);

private:
    void* impl_ = nullptr;     // opaque tsf*
    bool  owns_buffer_ = false;
    void* buffer_      = nullptr;
};

// Embedded-friendly SFZ player.
//
// Parses an SFZ file (subset listed below), opens each region's sample as a
// StreamingSampleSource (so big libraries don't have to fit in RAM), and
// drives a MultisamplePlayer for polyphonic playback.
//
// Why not sfizz? sfizz is desktop-class (C++17, libsndfile, abseil, MB+ of
// code) — great on Linux/embedded-Linux, too heavy for bare-metal RP2350.
// This implementation covers the subset that matters for sample libraries:
//
//   <region> blocks
//   sample=<path>            (relative to the .sfz directory)
//   lokey=<midi> hikey=<midi> key=<midi>      (key=N sets lo/hi/center to N)
//   pitch_keycenter=<midi>
//   lovel=<v>    hivel=<v>
//   loop_start=<frame>       loop_end=<frame>
//   loop_mode=no_loop|one_shot|loop_continuous|loop_sustain
//   volume=<dB>              tune=<cents>     transpose=<semitones>
//
// <group> headers cascade their opcodes into following <region>s.
// <control>'s `default_path=` is honoured.

class SfzPlayer {
public:
    SfzPlayer();
    ~SfzPlayer();

    // Parse the SFZ, open each sample as a streaming source, populate the
    // contained MultisamplePlayer. Requires Storage to be mounted.
    bool load_from_sd(const char* path, std::size_t voice_count = 12);

    void close();
    bool loaded() const noexcept    { return zone_count_ > 0; }
    std::size_t zone_count() const noexcept { return zone_count_; }

    // MIDI handlers — forward straight to the contained player.
    void note_on (int channel, int note, int velocity);
    void note_off(int channel, int note);

    // Render frames into out_l/out_r (additive).
    void process(float* out_l, float* out_r, std::size_t frames);

    // For UI: peek at the underlying multisampler.
    MultisamplePlayer& player() noexcept { return player_; }

private:
    static constexpr std::size_t kMaxZones = 64;
    KeyZone           zones_[kMaxZones];
    SampleSource*     sources_[kMaxZones]{};
    std::size_t       zone_count_ = 0;
    MultisamplePlayer player_;
};

}  // namespace picoadk::dsp
