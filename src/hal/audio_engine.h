// Internal interface between audio.cpp and the PIO/DMA engine.

#pragma once

#include <cstddef>
#include <cstdint>

#include "picoadk/hal/audio.h"

namespace picoadk::audio_engine {

// Stand up the engine for the given config. Returns false if the requested
// configuration can't be honoured (e.g. asking for more slots than the PIO
// program supports, or asking for input on a board with no DIN pin).
bool setup(const AudioConfig& cfg);

// Tear down completely (frees DMA + PIO state machines).
void teardown();

void start();
void stop();
bool running();

// Most recent measured DSP load (0..1+; >1 = missed deadline).
float measured_load();

// Called from audio.cpp's user-facing set_callback / config wrappers — the
// engine pulls out the cached pointers when a buffer is ready.
struct UserCallbacks {
    AudioFloatCallback float_cb;
    AudioInt32Callback int_cb;
    void*              user;
};
void set_callbacks(const UserCallbacks& cb);

}  // namespace picoadk::audio_engine
