// Sample player + multisample manager.
//
// Three building blocks:
//   * MemorySampleSource     — entire WAV in RAM/PSRAM. Best latency, no SD I/O.
//   * StreamingSampleSource  — opens a WAV on SD; per-source prefetch ring so
//                               sequential voice playback never seeks.
//   * SamplePlayer           — one voice. Reads from any SampleSource via
//                               2-tap linear interp; pitch + loop + speed.
//   * MultisamplePlayer      — voice pool over a KeyZone[] keymap. Per-instance
//                               state — multiple players coexist.
//
// Streaming polyphony — design notes
// -----------------------------------------------------------------------------
// MultisamplePlayer voices each call SampleSource::read(pos, ...) on whatever
// source their zone points at. With a single shared StreamingSampleSource and
// N voices reading at different positions you'd thrash the file pointer; the
// recommended pattern is one StreamingSampleSource per zone (which SfzPlayer
// follows). The per-source prefetch ring (`kPrefetchFrames` planar samples)
// catches sequential reads — typical sustained playback hits the cache 100 %
// of the time and only seeks once at note-on.

#include "picoadk/dsp/sample_player.h"
#include "picoadk/dsp/real.h"
#include "picoadk/hal/storage.h"
#include "picoadk/hal/psram.h"

#include <cmath>
#include <cstdlib>
#include <cstring>

namespace picoadk::dsp {

// ---- MemorySampleSource -------------------------------------------------
MemorySampleSource* MemorySampleSource::load_wav_from_memory(const uint8_t* data, std::size_t bytes) {
    if (bytes < 44) return nullptr;
    if (memcmp(data, "RIFF", 4) || memcmp(data + 8, "WAVE", 4)) return nullptr;
    uint16_t channels = (uint16_t)(data[22] | (data[23] << 8));
    uint32_t sr       = (uint32_t)(data[24] | (data[25] << 8) | (data[26] << 16) | (data[27] << 24));
    uint16_t bits     = (uint16_t)(data[34] | (data[35] << 8));
    const uint8_t* pcm = nullptr;
    std::size_t pcm_bytes = 0;
    for (std::size_t i = 12; i + 8 < bytes; ) {
        uint32_t chunk_size = (uint32_t)(data[i+4] | (data[i+5]<<8) | (data[i+6]<<16) | (data[i+7]<<24));
        if (!memcmp(data + i, "data", 4)) { pcm = data + i + 8; pcm_bytes = chunk_size; break; }
        i += 8 + chunk_size;
    }
    if (!pcm) return nullptr;

    auto* obj = new MemorySampleSource();
    obj->ch_   = channels;
    obj->sr_   = sr;
    obj->len_  = pcm_bytes / (channels * (bits / 8));
    obj->data_ = (Real**)std::calloc(channels, sizeof(Real*));
    for (uint16_t c = 0; c < channels; ++c)
        obj->data_[c] = (Real*)std::calloc(obj->len_, sizeof(Real));

    for (std::size_t f = 0; f < obj->len_; ++f) {
        for (uint16_t c = 0; c < channels; ++c) {
            float s = 0.0f;
            std::size_t off = (f * channels + c) * (bits / 8);
            switch (bits) {
                case 16: { int16_t v = (int16_t)(pcm[off] | (pcm[off+1] << 8));
                           s = v / 32768.0f; } break;
                case 24: { int32_t v = (int32_t)(pcm[off] | (pcm[off+1] << 8) | (pcm[off+2] << 16));
                           if (v & 0x800000) v |= ~0xFFFFFF;
                           s = v / 8388608.0f; } break;
                case 32: { int32_t v = (int32_t)(pcm[off] | (pcm[off+1]<<8) | (pcm[off+2]<<16) | (pcm[off+3]<<24));
                           s = v / 2147483648.0f; } break;
            }
            obj->data_[c][f] = from_float(s);
        }
    }
    return obj;
}

MemorySampleSource* MemorySampleSource::load_wav_from_sd(const char* path) {
    if (!Storage::is_mounted()) return nullptr;
    constexpr std::size_t kCap = 4 * 1024 * 1024;     // 4 MB cap, fits in PSRAM
    static uint8_t* buf = nullptr;
    if (!buf) buf = (uint8_t*)std::malloc(kCap);
    if (!buf) return nullptr;
    std::size_t n = Storage::read_file(path, buf, kCap);
    return n ? load_wav_from_memory(buf, n) : nullptr;
}

MemorySampleSource::~MemorySampleSource() {
    if (data_) {
        for (std::size_t c = 0; c < ch_; ++c) std::free(data_[c]);
        std::free(data_);
    }
}

std::size_t MemorySampleSource::read(std::size_t pos, Real** out, std::size_t frames) {
    if (pos >= len_) return 0;
    std::size_t n = std::min(frames, len_ - pos);
    for (std::size_t c = 0; c < ch_; ++c) std::memcpy(out[c], data_[c] + pos, n * sizeof(Real));
    return n;
}

// ---- StreamingSampleSource ----------------------------------------------

namespace {
constexpr std::size_t kPrefetchFrames = 4096;     // ~32 KB per channel @ float

struct StreamImpl {
#if PICOADK_HAS_SDFAT
    FsFile        file;
#endif
    std::size_t   channels    = 2;
    std::size_t   length      = 0;
    std::size_t   sample_rate = 48000;
    std::size_t   bits        = 16;
    std::size_t   data_offset = 0;
    std::size_t   read_pos    = 0;     // file pos in frames

    Real*         cache[2]    = {nullptr, nullptr};
    std::size_t   cache_start = 0;     // first cached frame
    std::size_t   cache_end   = 0;     // exclusive end frame; ==start ⇒ empty
};

void free_cache(StreamImpl* impl) {
    for (auto*& p : impl->cache) {
        if (!p) continue;
        if (Psram::available()) Psram::free(p); else std::free(p);
        p = nullptr;
    }
}
}  // anonymous

StreamingSampleSource* StreamingSampleSource::open_wav(const char* path,
                                                        std::size_t /*prefetch_frames*/) {
#if PICOADK_HAS_SDFAT
    if (!Storage::is_mounted()) return nullptr;
    auto* obj = new StreamingSampleSource();
    auto* impl = new StreamImpl();
    obj->impl_ = impl;
    if (!impl->file.open(path, O_RDONLY)) { delete impl; delete obj; return nullptr; }

    uint8_t hdr[44];
    if (impl->file.read(hdr, 44) != 44) { impl->file.close(); delete impl; delete obj; return nullptr; }
    if (memcmp(hdr, "RIFF", 4) || memcmp(hdr + 8, "WAVE", 4)) {
        impl->file.close(); delete impl; delete obj; return nullptr;
    }
    impl->channels    = (uint16_t)(hdr[22] | (hdr[23] << 8));
    impl->sample_rate = (uint32_t)(hdr[24] | (hdr[25]<<8) | (hdr[26]<<16) | (hdr[27]<<24));
    impl->bits        = (uint16_t)(hdr[34] | (hdr[35] << 8));
    if (!impl->channels || !impl->bits) {
        impl->file.close(); delete impl; delete obj; return nullptr;
    }

    impl->file.seekSet(12);
    char chunk_id[4];
    uint32_t chunk_sz;
    while (impl->file.available()) {
        if (impl->file.read(chunk_id, 4) != 4) break;
        if (impl->file.read(&chunk_sz, 4) != 4) break;
        if (memcmp(chunk_id, "data", 4) == 0) {
            impl->data_offset = (std::size_t)impl->file.curPosition();
            impl->length      = chunk_sz / (impl->channels * (impl->bits / 8));
            break;
        }
        impl->file.seekCur(chunk_sz);
    }
    if (!impl->data_offset) { impl->file.close(); delete impl; delete obj; return nullptr; }

    // Per-channel prefetch buffer. PSRAM-first.
    for (std::size_t c = 0; c < impl->channels && c < 2; ++c) {
        std::size_t bytes = kPrefetchFrames * sizeof(Real);
        Real* mem = nullptr;
        if (Psram::available()) mem = (Real*)Psram::alloc(bytes, 8);
        if (!mem)               mem = (Real*)std::calloc(kPrefetchFrames, sizeof(Real));
        impl->cache[c] = mem;
    }
    return obj;
#else
    (void)path;
    return nullptr;
#endif
}

StreamingSampleSource::~StreamingSampleSource() {
#if PICOADK_HAS_SDFAT
    if (impl_) {
        auto* impl = static_cast<StreamImpl*>(impl_);
        free_cache(impl);
        impl->file.close();
        delete impl;
    }
#endif
}
std::size_t StreamingSampleSource::channels()       const {
    return impl_ ? static_cast<StreamImpl*>(impl_)->channels    : 0;
}
std::size_t StreamingSampleSource::length()         const {
    return impl_ ? static_cast<StreamImpl*>(impl_)->length      : 0;
}
std::size_t StreamingSampleSource::sample_rate_hz() const {
    return impl_ ? static_cast<StreamImpl*>(impl_)->sample_rate : 0;
}

#if PICOADK_HAS_SDFAT
namespace {
// Refill the per-source prefetch ring starting at `pos`. Returns frames
// actually loaded (up to kPrefetchFrames or end-of-file).
std::size_t refill_cache(StreamImpl* impl, std::size_t pos) {
    if (pos >= impl->length) { impl->cache_start = impl->cache_end = pos; return 0; }
    std::size_t want = std::min<std::size_t>(kPrefetchFrames, impl->length - pos);

    std::size_t sample_bytes = impl->bits / 8;
    impl->file.seekSet(impl->data_offset + pos * impl->channels * sample_bytes);

    static uint8_t scratch[2048];
    std::size_t loaded = 0;
    while (loaded < want) {
        std::size_t blk_frames = std::min<std::size_t>(want - loaded,
                                                       sizeof(scratch) / (impl->channels * sample_bytes));
        std::size_t blk_bytes  = blk_frames * impl->channels * sample_bytes;
        int got = impl->file.read(scratch, (uint32_t)blk_bytes);
        if (got <= 0) break;
        std::size_t got_frames = (std::size_t)got / (impl->channels * sample_bytes);
        for (std::size_t f = 0; f < got_frames; ++f) {
            for (std::size_t c = 0; c < impl->channels && c < 2; ++c) {
                std::size_t off = (f * impl->channels + c) * sample_bytes;
                float s = 0.0f;
                switch (impl->bits) {
                    case 16: { int16_t v = (int16_t)(scratch[off] | (scratch[off+1] << 8));
                               s = v / 32768.0f; } break;
                    case 24: { int32_t v = (int32_t)(scratch[off] | (scratch[off+1]<<8) | (scratch[off+2]<<16));
                               if (v & 0x800000) v |= ~0xFFFFFF;
                               s = v / 8388608.0f; } break;
                    case 32: { int32_t v = (int32_t)(scratch[off] | (scratch[off+1]<<8) | (scratch[off+2]<<16) | (scratch[off+3]<<24));
                               s = v / 2147483648.0f; } break;
                }
                if (impl->cache[c]) impl->cache[c][loaded + f] = from_float(s);
            }
        }
        loaded += got_frames;
        if (got_frames < blk_frames) break;
    }
    impl->cache_start = pos;
    impl->cache_end   = pos + loaded;
    impl->read_pos    = impl->cache_end;
    return loaded;
}
}  // anonymous
#endif

std::size_t StreamingSampleSource::read(std::size_t pos, Real** out, std::size_t frames) {
#if PICOADK_HAS_SDFAT
    if (!impl_) return 0;
    auto* impl = static_cast<StreamImpl*>(impl_);
    if (pos >= impl->length) return 0;

    // Cache hit?
    if (pos < impl->cache_start || pos >= impl->cache_end) {
        if (refill_cache(impl, pos) == 0) return 0;
    }

    std::size_t avail   = impl->cache_end - pos;
    std::size_t n       = std::min(frames, avail);
    std::size_t cache_i = pos - impl->cache_start;
    for (std::size_t c = 0; c < impl->channels && c < 2; ++c)
        if (impl->cache[c])
            std::memcpy(out[c], impl->cache[c] + cache_i, n * sizeof(Real));
    return n;
#else
    (void)pos; (void)out; (void)frames;
    return 0;
#endif
}

// ---- SamplePlayer -------------------------------------------------------
void SamplePlayer::reset(float sr)             { sr_ = sr; pos_ = 0; active_ = false; }
void SamplePlayer::set_source(SampleSource* s) { src_ = s; pos_ = 0; }
void SamplePlayer::set_speed(float s)          { speed_ = s; }
void SamplePlayer::set_pitch_semitones(float n){ speed_ = std::pow(2.0f, n / 12.0f); }
void SamplePlayer::set_loop(bool l, std::size_t a, std::size_t b) { loop_ = l; loop_a_ = a; loop_b_ = b; }
void SamplePlayer::set_interp(SamplerInterp i) { interp_ = i; }
void SamplePlayer::set_time_stretch(bool on, float r) { stretch_ = on; stretch_ratio_ = r; }
void SamplePlayer::trigger() { pos_ = 0; active_ = true; }
void SamplePlayer::stop()    { active_ = false; }
bool SamplePlayer::active() const noexcept { return active_; }

void SamplePlayer::process(Real* out_l, Real* out_r, std::size_t frames) {
    if (!active_ || !src_) return;
    float src_sr = src_->sample_rate_hz();
    float step   = (src_sr / sr_) * speed_;
    std::size_t channels = src_->channels();
    Real lbuf[4], rbuf[4]; Real* p[2] = { lbuf, rbuf };
    for (std::size_t i = 0; i < frames; ++i) {
        std::size_t a = (std::size_t)pos_;
        if (loop_ && loop_b_ > loop_a_ && a >= loop_b_) {
            pos_ = (float)loop_a_;
            a    = loop_a_;
        }
        if (a >= src_->length()) { active_ = false; break; }
        std::size_t n = src_->read(a, p, 2);
        if (n < 2) break;
        float frac = pos_ - (float)a;
        float l = (1.0f - frac) * to_float(lbuf[0]) + frac * to_float(lbuf[1]);
        float r = (channels >= 2) ? ((1.0f - frac) * to_float(rbuf[0]) + frac * to_float(rbuf[1])) : l;
        out_l[i] = from_float(to_float(out_l[i]) + l);
        out_r[i] = from_float(to_float(out_r[i]) + r);
        pos_ += step;
    }
}

// ---- MultisamplePlayer (per-instance state) -----------------------------
namespace {
const KeyZone* find_zone_for(const KeyZone* zones, std::size_t count, uint8_t note) {
    const KeyZone* best = nullptr;
    int best_span = 0x7FFF;
    for (std::size_t i = 0; i < count; ++i) {
        const KeyZone& z = zones[i];
        if (note < z.lo_note || note > z.hi_note) continue;
        int span = (int)z.hi_note - (int)z.lo_note;
        if (span < best_span) { best = &z; best_span = span; }
    }
    return best;
}
}  // anonymous

void MultisamplePlayer::reset(float sr, std::size_t voice_count) {
    sr_           = sr;
    voice_count_  = (voice_count < kMaxVoices) ? voice_count : kMaxVoices;
    age_counter_  = 0;
    for (std::size_t i = 0; i < voice_count_; ++i) {
        voices_[i] = {};
        voices_[i].player.reset(sr);
    }
}

void MultisamplePlayer::set_zones(const KeyZone* zones, std::size_t count) {
    zones_      = zones;
    zone_count_ = count;
}

void MultisamplePlayer::note_on(uint8_t note, uint8_t velocity) {
    const KeyZone* z = find_zone_for(zones_, zone_count_, note);
    if (!z || !z->source) return;

    Voice* victim = nullptr;
    for (std::size_t i = 0; i < voice_count_; ++i)
        if (!voices_[i].active) { victim = &voices_[i]; break; }
    if (!victim) {
        victim = &voices_[0];
        for (std::size_t i = 1; i < voice_count_; ++i)
            if (voices_[i].age < victim->age) victim = &voices_[i];
    }
    victim->note   = note;
    victim->active = true;
    victim->age    = ++age_counter_;
    victim->gain   = z->gain * (velocity / 127.0f);
    victim->player.set_source(z->source);
    victim->player.set_pitch_semitones((float)((int)note - (int)z->root_note));
    victim->player.set_loop(false);
    victim->player.trigger();
}

void MultisamplePlayer::note_off(uint8_t note) {
    for (std::size_t i = 0; i < voice_count_; ++i) {
        if (voices_[i].active && voices_[i].note == note) {
            voices_[i].player.stop();
            voices_[i].active = false;
        }
    }
}

void MultisamplePlayer::process(Real* out_l, Real* out_r, std::size_t frames) {
    for (std::size_t f = 0; f < frames; ++f) { out_l[f] = 0; out_r[f] = 0; }
    for (std::size_t i = 0; i < voice_count_; ++i) {
        if (!voices_[i].active) continue;
        Real scratch_l[128] = {0};
        Real scratch_r[128] = {0};
        std::size_t blk = frames > 128 ? 128 : frames;
        voices_[i].player.process(scratch_l, scratch_r, blk);
        const float g = voices_[i].gain;
        for (std::size_t f = 0; f < blk; ++f) {
            out_l[f] = from_float(to_float(out_l[f]) + to_float(scratch_l[f]) * g);
            out_r[f] = from_float(to_float(out_r[f]) + to_float(scratch_r[f]) * g);
        }
        if (!voices_[i].player.active()) voices_[i].active = false;
    }
}

std::size_t MultisamplePlayer::voices_active() const noexcept {
    std::size_t n = 0;
    for (std::size_t i = 0; i < voice_count_; ++i) if (voices_[i].active) ++n;
    return n;
}

}  // namespace picoadk::dsp
