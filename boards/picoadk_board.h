// Board selector — chooses the correct pin map.
// CMake defines PICOADK_BOARD_V1 or PICOADK_BOARD_V2 via add_compile_definitions.
// Code should include this header (or <picoadk/picoadk.h>) and never include
// the per-board headers directly.

#pragma once

#if defined(PICOADK_BOARD_SELECT_V1)
#  include "picoadk_v1.h"
#elif defined(PICOADK_BOARD_SELECT_V2)
#  include "picoadk_v2.h"
#else
#  error "No PicoADK board selected. Set -DPICOADK_BOARD=v1_rp2040 or v2_rp2350."
#endif
