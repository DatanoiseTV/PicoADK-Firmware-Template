// PicoADK — umbrella header.
//
// Include this from application code to pull in the full HAL surface.
// Individual subsystems remain available as <picoadk/hal/audio.h> etc. for
// users who prefer narrower includes.

#pragma once

#include "picoadk_board.h"

#include "picoadk/hal/system.h"
#include "picoadk/hal/pins.h"
#include "picoadk/hal/audio.h"
#include "picoadk/hal/midi.h"
#include "picoadk/hal/controls.h"
#include "picoadk/hal/storage.h"
#include "picoadk/hal/psram.h"
#include "picoadk/hal/gfx.h"
#include "picoadk/hal/display.h"
#include "picoadk/hal/encoders.h"
#include "picoadk/hal/buttons.h"
#include "picoadk/hal/mux.h"
#include "picoadk/hal/usb_device.h"
#include "picoadk/hal/usb_host.h"
#include "picoadk/hal/voicelink.h"
#include "picoadk/hal/hardware.h"

#include "picoadk/midi/general_handler.h"
#include "picoadk/midi/launchpad.h"

#include "picoadk/dsp/real.h"
#include "picoadk/dsp/math.h"
#include "picoadk/dsp/modules.h"
#include "picoadk/dsp/filters.h"
#include "picoadk/dsp/effects.h"
#include "picoadk/dsp/sample_player.h"
#include "picoadk/dsp/sf2.h"
#include "picoadk/dsp/pd.h"
