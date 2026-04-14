# Board selection.
#
# Pick the target hardware with -DPICOADK_BOARD=<id> at configure time:
#   v1_rp2040  -> PicoADK v1 (RP2040, original board)
#   v2_rp2350  -> PicoADK v2 (RP2350, M33 + PSRAM + SDIO)
#
# This file is included before project() so PICO_PLATFORM / PICO_BOARD are set
# in time for pico_sdk_init().

if(NOT DEFINED PICOADK_BOARD)
    set(PICOADK_BOARD "v1_rp2040" CACHE STRING "PicoADK target board")
endif()

set_property(CACHE PICOADK_BOARD PROPERTY STRINGS "v1_rp2040" "v2_rp2350")

# Custom board headers live in <repo>/boards/. Pico-sdk picks them up via
# PICO_BOARD_HEADER_DIRS. The board name (e.g. datanoisetv_rp2040_dsp) maps to
# boards/datanoisetv_rp2040_dsp.h.
set(PICO_BOARD_HEADER_DIRS "${CMAKE_CURRENT_LIST_DIR}/../../boards" CACHE PATH "" FORCE)

if(PICOADK_BOARD STREQUAL "v1_rp2040")
    set(PICO_PLATFORM "rp2040" CACHE STRING "" FORCE)
    set(PICO_BOARD    "datanoisetv_rp2040_dsp" CACHE STRING "" FORCE)
    set(PICOADK_BOARD_DEF "PICOADK_BOARD_SELECT_V1")
    message(STATUS "PicoADK board: v1 (RP2040)")
elseif(PICOADK_BOARD STREQUAL "v2_rp2350")
    set(PICO_PLATFORM "rp2350-arm-s" CACHE STRING "" FORCE)
    set(PICO_BOARD    "datanoisetv_rp2350_dsp" CACHE STRING "" FORCE)
    set(PICOADK_BOARD_DEF "PICOADK_BOARD_SELECT_V2")
    message(STATUS "PicoADK board: v2 (RP2350)")
else()
    message(FATAL_ERROR
        "Unknown PICOADK_BOARD '${PICOADK_BOARD}'. Use 'v1_rp2040' or 'v2_rp2350'.")
endif()
