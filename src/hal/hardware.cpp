// Hardware bring-up convenience wrapper.

#include "picoadk/hal/hardware.h"

namespace picoadk::Hardware {

bool init(const HardwareConfig& cfg) {
    System::init(cfg.system);

    if (cfg.init_psram)    Psram::init();
    if (cfg.init_controls) Controls::init();
    if (cfg.init_midi)     Midi::init(cfg.midi);
    if (cfg.init_audio)    Audio::init(cfg.audio);
    if (cfg.init_storage)  Storage::init(cfg.storage);
    if (cfg.init_display)  Display::init(cfg.display);

    return true;
}

}  // namespace picoadk::Hardware
