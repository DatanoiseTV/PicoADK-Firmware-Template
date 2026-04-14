// Internal interface between transports and the dispatcher in midi.cpp.

#pragma once

#include "picoadk/hal/midi.h"

namespace picoadk::Midi {

void dispatch(const MidiMessage& m);

}  // namespace picoadk::Midi
