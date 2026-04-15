// Audio HAL front-end — owns config and callback dispatch; defers PIO+DMA
// plumbing to audio_engine.cpp.

#include "picoadk/hal/audio.h"
#include "audio_engine.h"

namespace picoadk::Audio {

namespace {
AudioConfig         g_cfg;
AudioFloatCallback  g_float_cb = nullptr;
AudioInt32Callback  g_int_cb   = nullptr;
void*               g_user     = nullptr;
}

bool init(const AudioConfig& cfg) {
    g_cfg = cfg;
    return audio_engine::setup(cfg);
}

void set_callback(AudioFloatCallback cb, void* user) {
    g_float_cb = cb;
    g_int_cb   = nullptr;
    g_user     = user;
    audio_engine::set_callbacks({cb, nullptr, user});
}
void set_callback(AudioInt32Callback cb, void* user) {
    g_int_cb   = cb;
    g_float_cb = nullptr;
    g_user     = user;
    audio_engine::set_callbacks({nullptr, cb, user});
}

void start()                          { audio_engine::start(); }
void stop()                           { audio_engine::stop();  }
bool is_running()                     { return audio_engine::running(); }
const AudioConfig& config()           { return g_cfg; }
float load()                          { return audio_engine::measured_load(); }

bool reconfigure(const AudioConfig& cfg) {
    bool was_running = is_running();
    if (was_running) stop();
    audio_engine::teardown();
    bool ok = init(cfg);
    if (was_running && ok) {
        if (g_float_cb) audio_engine::set_callbacks({g_float_cb, nullptr, g_user});
        else if (g_int_cb) audio_engine::set_callbacks({nullptr, g_int_cb, g_user});
        start();
    }
    return ok;
}

}  // namespace picoadk::Audio
