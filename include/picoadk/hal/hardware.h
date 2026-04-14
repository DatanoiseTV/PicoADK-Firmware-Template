// Top-level convenience entry point.
//
//     picoadk::Hardware::init();           // brings everything up with defaults
//     picoadk::Hardware::audio().start();  // starts the audio engine
//
// Each subsystem can also be initialised individually if you want to skip
// pieces (e.g. headless firmware with no display).

#pragma once

#include "picoadk/hal/system.h"
#include "picoadk/hal/audio.h"
#include "picoadk/hal/midi.h"
#include "picoadk/hal/controls.h"
#include "picoadk/hal/storage.h"
#include "picoadk/hal/psram.h"
#include "picoadk/hal/display.h"

namespace picoadk {

struct HardwareConfig {
    SystemConfig  system;
    AudioConfig   audio;
    MidiConfig    midi;
    StorageConfig storage;
    DisplayConfig display;

    bool init_audio    = true;
    bool init_midi     = true;
    bool init_controls = true;
    bool init_storage  = false;     // off by default (requires SD card)
    bool init_display  = false;     // off by default (requires panel)
    bool init_psram    = true;
};

namespace Hardware {

// Bring everything up. Returns false if a *required* subsystem failed.
bool init(const HardwareConfig& cfg = {});

}  // namespace Hardware
}  // namespace picoadk
