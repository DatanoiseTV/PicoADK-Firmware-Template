// SoundFont 2 / SFZ playback.
//
// TinySoundFont (single-header) lives at lib/TinySoundFont/tsf.h and is
// pulled in here as the single TU that defines TSF_IMPLEMENTATION. Exposing
// it as a regular C++ class (not header-only) keeps build times sane and
// lets us hide the MIDI-channel state inside a small class.
//
// SFZ parsing is more work than fits this commit — the SfzPlayer stub
// reserves the API; PRs welcome.

#include "picoadk/dsp/sf2.h"
#include "picoadk/hal/storage.h"
#include "picoadk/hal/psram.h"

#include <cstdlib>
#include <cstring>

#define TSF_IMPLEMENTATION
#define TSF_NO_STDIO         // no fopen — we feed buffers ourselves
#include "tsf.h"

namespace picoadk::dsp {

Sf2Player::Sf2Player()  = default;
Sf2Player::~Sf2Player() { close(); }

bool Sf2Player::load_from_memory(const void* data, std::size_t bytes) {
    close();
    impl_ = tsf_load_memory(const_cast<void*>(data), (int)bytes);
    return impl_ != nullptr;
}

bool Sf2Player::load_from_sd(const char* path) {
    close();
#if PICOADK_HAS_SDFAT
    if (!Storage::is_mounted()) return false;
    // Allocate the buffer from PSRAM if available — soundfonts are big.
    extern std::size_t Storage_card_size_bytes();
    // Read in 64 KB chunks until file is consumed (no good way to know up
    // front without stat()). We size the read buffer to 4 MB max, which is
    // typical for general MIDI fonts; users with bigger fonts should call
    // load_from_memory() against a PSRAM-resident buffer they own.
    constexpr std::size_t kMaxFontBytes = 4u * 1024 * 1024;
    void* buf = nullptr;
    if (Psram::available()) buf = Psram::alloc(kMaxFontBytes, 8);
    if (!buf)               buf = std::malloc(kMaxFontBytes);
    if (!buf) return false;

    std::size_t got = Storage::read_file(path, buf, kMaxFontBytes);
    if (!got) {
        if (Psram::available()) Psram::free(buf); else std::free(buf);
        return false;
    }

    impl_ = tsf_load_memory(buf, (int)got);
    if (!impl_) {
        if (Psram::available()) Psram::free(buf); else std::free(buf);
        return false;
    }
    owns_buffer_ = true;
    buffer_      = buf;
    return true;
#else
    (void)path;
    return false;
#endif
}

void Sf2Player::close() {
    if (impl_) { tsf_close((tsf*)impl_); impl_ = nullptr; }
    if (owns_buffer_ && buffer_) {
        if (Psram::available()) Psram::free(buffer_); else std::free(buffer_);
    }
    owns_buffer_ = false;
    buffer_      = nullptr;
}

bool Sf2Player::loaded() const noexcept    { return impl_ != nullptr; }
int  Sf2Player::preset_count() const noexcept {
    return impl_ ? tsf_get_presetcount((tsf*)impl_) : 0;
}

void Sf2Player::set_preset(int bank, int preset_number) {
    if (!impl_) return;
    int idx = tsf_get_presetindex((tsf*)impl_, bank, preset_number);
    if (idx < 0) idx = 0;
    tsf_set_max_voices((tsf*)impl_, 24);
    tsf_channel_set_presetindex((tsf*)impl_, 0, idx);
}

void Sf2Player::set_sample_rate(float sr) {
    if (!impl_) return;
    tsf_set_output((tsf*)impl_, TSF_STEREO_INTERLEAVED, (int)sr, 0.0f);
}

void Sf2Player::set_gain(float linear) {
    if (!impl_) return;
    tsf_set_volume((tsf*)impl_, linear);
}

void Sf2Player::note_on (int ch, int note, float vel) {
    if (impl_) tsf_channel_note_on((tsf*)impl_, ch, note, vel);
}
void Sf2Player::note_off(int ch, int note) {
    if (impl_) tsf_channel_note_off((tsf*)impl_, ch, note);
}
void Sf2Player::all_notes_off(int ch) {
    if (impl_) tsf_channel_note_off_all((tsf*)impl_, ch);
}
void Sf2Player::program_change(int ch, int preset) {
    if (impl_) tsf_channel_set_presetnumber((tsf*)impl_, ch, preset, 0);
}
void Sf2Player::pitch_bend(int ch, float semis) {
    if (impl_) tsf_channel_set_pitchwheel((tsf*)impl_, ch, (int)(8192 + semis * 8192.0f / 2.0f));
}
void Sf2Player::cc(int ch, int controller, int value) {
    if (impl_) tsf_channel_midi_control((tsf*)impl_, ch, controller, value);
}

void Sf2Player::process(float* out_l, float* out_r, std::size_t frames) {
    if (!impl_) return;
    // TinySoundFont renders interleaved — render to a scratch and split.
    constexpr std::size_t kScratch = 128;
    float scratch[kScratch * 2];
    std::size_t done = 0;
    while (done < frames) {
        std::size_t blk = (frames - done) > kScratch ? kScratch : (frames - done);
        tsf_render_float((tsf*)impl_, scratch, (int)blk, 0);
        for (std::size_t i = 0; i < blk; ++i) {
            out_l[done + i] += scratch[i * 2 + 0];
            out_r[done + i] += scratch[i * 2 + 1];
        }
        done += blk;
    }
}

// ---- SfzPlayer ----------------------------------------------------------
//
// Single-pass tokeniser/parser over the SFZ text format. Handles cascading
// <group> opcodes, <control>::default_path, and the most-used <region>
// opcodes. Comments (// ...) are stripped.

namespace {

struct SfzOpcodes {
    char     sample_path[128] = {0};
    int      lokey   = 0,   hikey   = 127;
    int      lovel   = 0,   hivel   = 127;
    int      keycenter = 60;
    int      transpose = 0;        // semitones
    float    tune_cents = 0.0f;
    float    volume_db  = 0.0f;
    int      loop_start = -1;
    int      loop_end   = -1;
    enum LoopMode { LM_None, LM_OneShot, LM_Continuous, LM_Sustain };
    LoopMode loop_mode = LM_None;
    bool     have_loop_mode = false;
};

void apply_op(SfzOpcodes& op, const char* k, const char* v) {
    auto eq = [](const char* a, const char* b) { return std::strcmp(a, b) == 0; };
    if      (eq(k, "sample"))           std::strncpy(op.sample_path, v, sizeof(op.sample_path) - 1);
    else if (eq(k, "lokey"))            op.lokey = std::atoi(v);
    else if (eq(k, "hikey"))            op.hikey = std::atoi(v);
    else if (eq(k, "key")) {            int kk = std::atoi(v); op.lokey = op.hikey = op.keycenter = kk; }
    else if (eq(k, "pitch_keycenter"))  op.keycenter = std::atoi(v);
    else if (eq(k, "lovel"))            op.lovel = std::atoi(v);
    else if (eq(k, "hivel"))            op.hivel = std::atoi(v);
    else if (eq(k, "transpose"))        op.transpose = std::atoi(v);
    else if (eq(k, "tune"))             op.tune_cents = (float)std::atof(v);
    else if (eq(k, "volume"))           op.volume_db = (float)std::atof(v);
    else if (eq(k, "loop_start"))       op.loop_start = std::atoi(v);
    else if (eq(k, "loop_end"))         op.loop_end = std::atoi(v);
    else if (eq(k, "loop_mode")) {
        op.have_loop_mode = true;
        if      (eq(v, "no_loop"))          op.loop_mode = SfzOpcodes::LM_None;
        else if (eq(v, "one_shot"))         op.loop_mode = SfzOpcodes::LM_OneShot;
        else if (eq(v, "loop_continuous"))  op.loop_mode = SfzOpcodes::LM_Continuous;
        else if (eq(v, "loop_sustain"))     op.loop_mode = SfzOpcodes::LM_Sustain;
    }
    // Unknown opcodes are silently ignored — the SFZ spec is huge and
    // sample libraries reference many vendor extensions.
}

// Parse `key=value` pairs from `cur` until end of section.
const char* parse_kv(const char* cur, const char* end, SfzOpcodes& op) {
    while (cur < end && (*cur == ' ' || *cur == '\t' || *cur == '\n' || *cur == '\r')) cur++;
    if (cur >= end || *cur == '<') return cur;

    char key[64], val[256];
    std::size_t kl = 0, vl = 0;
    while (cur < end && *cur != '=' && *cur != ' ' && *cur != '\n' && kl < sizeof(key) - 1) key[kl++] = *cur++;
    key[kl] = 0;
    if (cur >= end || *cur != '=') return cur + 1;
    cur++;
    // Value runs to the next opcode-start or section header. SFZ values can
    // contain spaces (e.g. file paths with " ") but they end before the next
    // `<key>=` pair. We read until we see ` <letter>=` or `<` or end-of-line.
    while (cur < end && *cur != '<' && *cur != '\n' && *cur != '\r' && vl < sizeof(val) - 1) {
        val[vl++] = *cur++;
    }
    while (vl > 0 && (val[vl - 1] == ' ' || val[vl - 1] == '\t')) vl--;
    val[vl] = 0;
    apply_op(op, key, val);
    return cur;
}

void apply_zone(KeyZone& z, const SfzOpcodes& op, SampleSource* src) {
    z.source    = src;
    z.lo_note   = (uint8_t)op.lokey;
    z.hi_note   = (uint8_t)op.hikey;
    z.root_note = (uint8_t)op.keycenter;
    z.gain      = std::pow(10.0f, op.volume_db / 20.0f);
}

// Resolve `default_path` + `sample=` against the SFZ file's directory.
void make_path(char* dst, std::size_t cap, const char* sfz_dir, const char* def_path, const char* sample) {
    dst[0] = 0;
    std::size_t n = 0;
    auto cat = [&](const char* s) {
        while (*s && n < cap - 1) dst[n++] = *s++;
        dst[n] = 0;
    };
    if (sfz_dir[0]) { cat(sfz_dir); if (n && dst[n-1] != '/') cat("/"); }
    if (def_path[0]) { cat(def_path); if (n && dst[n-1] != '/') cat("/"); }
    cat(sample);
}

}  // anonymous

SfzPlayer::SfzPlayer()  = default;
SfzPlayer::~SfzPlayer() { close(); }

void SfzPlayer::close() {
    for (std::size_t i = 0; i < zone_count_; ++i) {
        delete sources_[i];
        sources_[i] = nullptr;
    }
    zone_count_ = 0;
}

bool SfzPlayer::load_from_sd(const char* path, std::size_t voice_count) {
#if PICOADK_HAS_SDFAT
    if (!Storage::is_mounted()) return false;
    close();

    // Slurp the SFZ text.
    constexpr std::size_t kMaxSfz = 64 * 1024;
    char* text = (char*)std::malloc(kMaxSfz);
    if (!text) return false;
    std::size_t got = Storage::read_file(path, text, kMaxSfz - 1);
    if (!got) { std::free(text); return false; }
    text[got] = 0;

    // Strip // comments (in-place).
    for (std::size_t i = 0; i + 1 < got; ++i) {
        if (text[i] == '/' && text[i + 1] == '/') {
            while (i < got && text[i] != '\n') text[i++] = ' ';
        }
    }

    // SFZ dir — everything before the last '/'.
    char sfz_dir[64] = {0};
    {
        const char* slash = std::strrchr(path, '/');
        if (slash && slash != path) {
            std::size_t n = (std::size_t)(slash - path);
            if (n >= sizeof(sfz_dir)) n = sizeof(sfz_dir) - 1;
            std::memcpy(sfz_dir, path, n);
            sfz_dir[n] = 0;
        }
    }
    char default_path[64] = {0};

    SfzOpcodes group_op{};   // cascades into <region>s
    SfzOpcodes region_op;
    bool       in_region = false;

    auto commit_region = [&]() {
        if (!in_region || zone_count_ >= kMaxZones || !region_op.sample_path[0]) return;
        char abs_path[160];
        make_path(abs_path, sizeof(abs_path), sfz_dir, default_path, region_op.sample_path);
        // Replace backslashes with forward slashes (Windows-authored SFZs).
        for (char* p = abs_path; *p; ++p) if (*p == '\\') *p = '/';

        StreamingSampleSource* s = StreamingSampleSource::open_wav(abs_path);
        if (!s) { in_region = false; return; }
        sources_[zone_count_] = s;
        apply_zone(zones_[zone_count_], region_op, s);
        zone_count_++;
        in_region = false;
    };

    const char* cur = text;
    const char* end = text + got;
    while (cur < end) {
        while (cur < end && (*cur == ' ' || *cur == '\t' || *cur == '\n' || *cur == '\r')) cur++;
        if (cur >= end) break;

        if (*cur == '<') {
            commit_region();
            if (std::strncmp(cur, "<region>", 8) == 0) {
                region_op = group_op;     // inherit group
                in_region = true;
                cur += 8;
            } else if (std::strncmp(cur, "<group>", 7) == 0) {
                group_op = SfzOpcodes{};  // reset group
                cur += 7;
                while (cur < end && *cur != '<') cur = parse_kv(cur, end, group_op);
            } else if (std::strncmp(cur, "<control>", 9) == 0) {
                cur += 9;
                while (cur < end && *cur != '<') {
                    while (cur < end && (*cur == ' ' || *cur == '\t' || *cur == '\n')) cur++;
                    if (cur >= end || *cur == '<') break;
                    if (std::strncmp(cur, "default_path=", 13) == 0) {
                        cur += 13;
                        std::size_t n = 0;
                        while (cur < end && *cur != '\n' && *cur != '<' && n < sizeof(default_path) - 1)
                            default_path[n++] = *cur++;
                        default_path[n] = 0;
                    } else {
                        while (cur < end && *cur != '\n' && *cur != '<') cur++;
                    }
                }
            } else if (std::strncmp(cur, "<global>", 8) == 0) {
                cur += 8;
                while (cur < end && *cur != '<') cur = parse_kv(cur, end, group_op);
            } else {
                while (cur < end && *cur != '\n') cur++;
            }
            continue;
        }

        if (in_region)        cur = parse_kv(cur, end, region_op);
        else                  cur = parse_kv(cur, end, group_op);
    }
    commit_region();
    std::free(text);

    if (!zone_count_) return false;
    player_.reset(48000.0f, voice_count);
    player_.set_zones(zones_, zone_count_);
    return true;
#else
    (void)path; (void)voice_count;
    return false;
#endif
}

void SfzPlayer::note_on (int /*channel*/, int note, int velocity) {
    player_.note_on((uint8_t)note, (uint8_t)velocity);
}
void SfzPlayer::note_off(int /*channel*/, int note) {
    player_.note_off((uint8_t)note);
}
void SfzPlayer::process(float* out_l, float* out_r, std::size_t frames) {
    Real lbuf[128], rbuf[128];
    if (frames > 128) frames = 128;
    player_.process(lbuf, rbuf, frames);
    for (std::size_t i = 0; i < frames; ++i) {
        out_l[i] += to_float(lbuf[i]);
        out_r[i] += to_float(rbuf[i]);
    }
}

}  // namespace picoadk::dsp
