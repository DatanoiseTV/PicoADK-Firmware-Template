// PureData engine — headless libpd embedded in the firmware.
//
// Phase 6 vendors libpd and wires it up. Patches load from a memory buffer
// (handy for shipping a default patch in flash) or from the SD card.
//
// The PD instance acts as one audio block processor; it is invoked from the
// Audio callback. Multiple patches can run inside one instance via PD's
// dollarsign-namespacing. For multiple isolated PD instances use Pd::Engine
// objects directly (RAM permitting).

#pragma once

#include <cstddef>
#include <cstdint>

#include "picoadk/dsp/real.h"

namespace picoadk {

namespace Pd {

struct Config {
    uint32_t sample_rate_hz = 48000;
    uint16_t block_size     = 64;          // PD's internal block (must divide audio block)
    uint8_t  in_channels    = 2;
    uint8_t  out_channels   = 2;
};

bool init(const Config& cfg);

// Loads a patch. Returns an opaque handle (>0 on success).
// `argv_path` may be a directory on SD that PD will use to resolve abstractions.
int  load_patch_from_file (const char* sd_path, const char* argv_path = "/");
int  load_patch_from_memory(const void* buffer, std::size_t bytes,
                            const char* argv_path = "/");
void close_patch(int handle);

// Plumb messages.
void send_bang  (const char* receiver);
void send_float (const char* receiver, float value);
void send_symbol(const char* receiver, const char* symbol);
void send_note_on (uint8_t channel, uint8_t note, uint8_t velocity);
void send_note_off(uint8_t channel, uint8_t note);
void send_cc      (uint8_t channel, uint8_t cc,   uint8_t value);

// Subscribe to PD outputs. Return 0 to keep the message, non-zero to stop.
using FloatReceiver  = void (*)(const char* source, float v);
using SymbolReceiver = void (*)(const char* source, const char* sym);
using BangReceiver   = void (*)(const char* source);
void on_float (const char* source, FloatReceiver  cb);
void on_symbol(const char* source, SymbolReceiver cb);
void on_bang  (const char* source, BangReceiver   cb);

// Audio callback bridge. Call this from your Audio callback when using PD
// as the engine. `in` may be nullptr when not using audio input.
void process_block(const float* const* in, float* const* out, std::size_t frames);

}  // namespace Pd
}  // namespace picoadk
