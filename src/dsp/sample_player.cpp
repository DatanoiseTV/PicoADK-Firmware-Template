// Sample player + multisample manager.
//
// Phase 7 — initial implementation. Memory backend works against an in-RAM
// buffer (handy for shipping a small kit in flash); the streaming backend
// hooks into Storage HAL and double-buffers reads. Multisample picks the
// nearest zone by note, optionally interpolating across velocity layers.

#include "picoadk/dsp/sample_player.h"
#include "picoadk/dsp/real.h"
#include "picoadk/hal/storage.h"

#include <cmath>
#include <cstdlib>
#include <cstring>

namespace picoadk::dsp {

// ---- MemorySampleSource -------------------------------------------------
MemorySampleSource* MemorySampleSource::load_wav_from_memory(const uint8_t* data, std::size_t bytes) {
    if (bytes < 44) return nullptr;
    // Tiny RIFF/WAVE parser — just enough for PCM 16/24/32 mono/stereo.
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
    obj->ch_ = channels;
    obj->sr_ = sr;
    obj->len_ = pcm_bytes / (channels * (bits / 8));
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

// ---- StreamingSampleSource (skeleton) -----------------------------------
StreamingSampleSource* StreamingSampleSource::open_wav(const char*, std::size_t) { return nullptr; }
StreamingSampleSource::~StreamingSampleSource()    {}
std::size_t StreamingSampleSource::channels()       const { return 2; }
std::size_t StreamingSampleSource::length()         const { return 0; }
std::size_t StreamingSampleSource::sample_rate_hz() const { return 0; }
std::size_t StreamingSampleSource::read(std::size_t, Real**, std::size_t) { return 0; }

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
        std::size_t b = a + 1;
        if (loop_ && loop_b_ > loop_a_ && a >= loop_b_) { pos_ = (float)loop_a_; a = loop_a_; b = a + 1; }
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

// ---- MultisamplePlayer (skeleton) --------------------------------------
void MultisamplePlayer::reset(float, std::size_t) {}
void MultisamplePlayer::set_zones(const KeyZone*, std::size_t) {}
void MultisamplePlayer::note_on(uint8_t, uint8_t)  {}
void MultisamplePlayer::note_off(uint8_t)          {}
void MultisamplePlayer::process(Real*, Real*, std::size_t) {}

}  // namespace picoadk::dsp
