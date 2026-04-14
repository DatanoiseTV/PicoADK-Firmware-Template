// Audio HAL front-end. The PIO/DMA engine lives in audio_i2s.cpp; this file
// owns the user-facing config, callback dispatch, and float<->int conversion.
//
// PHASE 0 NOTE: the engine implementation is stubbed (engine_*) — Phase 1
// fills it in with the rewritten PIO state machines for I2S out, in, and
// full-duplex.

#include "picoadk/hal/audio.h"

#include <cstring>

namespace picoadk::Audio {

namespace {

AudioConfig         g_cfg;
AudioFloatCallback  g_float_cb = nullptr;
AudioInt32Callback  g_int_cb   = nullptr;
void*               g_user     = nullptr;
bool                g_running  = false;
float               g_load     = 0.0f;

}

bool init(const AudioConfig& cfg) {
    g_cfg = cfg;
    // engine_init() — fills in during Phase 1
    return true;
}

void set_callback(AudioFloatCallback cb, void* user) {
    g_float_cb = cb;
    g_int_cb   = nullptr;
    g_user     = user;
}

void set_callback(AudioInt32Callback cb, void* user) {
    g_int_cb   = cb;
    g_float_cb = nullptr;
    g_user     = user;
}

void start()                          { g_running = true;  /* engine_start(); */ }
void stop()                           { g_running = false; /* engine_stop();  */ }
bool is_running()                     { return g_running; }
const AudioConfig& config()           { return g_cfg; }
float load()                          { return g_load; }

bool reconfigure(const AudioConfig& cfg) {
    bool was_running = g_running;
    if (was_running) stop();
    bool ok = init(cfg);
    if (was_running && ok) start();
    return ok;
}

}  // namespace picoadk::Audio
