# PicoADK Firmware Template — top-level CMake.
#
# Configure with:
#   cmake -B build -DPICOADK_BOARD=v1_rp2040 -DPICOADK_APP=examples/02_synth_vult
#   cmake -B build -DPICOADK_BOARD=v2_rp2350 -DPICOADK_APP=examples/02_synth_vult
#
# The default app is examples/02_synth_vult (the original Vult monosynth so
# people who clone this repo still see the demo they were used to).
#
# Vendored versions:
#   pico-sdk        2.2.0     (lib/pico-sdk)
#   pico-extras     sdk-2.2.0 (lib/pico-extras)
#   FreeRTOS-Kernel V11.3.0   (lib/FreeRTOS-Kernel)

cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default the SDK / extras to the in-tree submodules unless the user already
# pointed at one (via env var or -D).
if(NOT DEFINED PICO_SDK_PATH AND NOT DEFINED ENV{PICO_SDK_PATH})
    set(PICO_SDK_PATH    "${CMAKE_CURRENT_LIST_DIR}/lib/pico-sdk"    CACHE PATH "")
endif()
if(NOT DEFINED PICO_EXTRAS_PATH AND NOT DEFINED ENV{PICO_EXTRAS_PATH})
    set(PICO_EXTRAS_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/pico-extras" CACHE PATH "")
endif()

# Choose the board *before* including pico_sdk_import.cmake so PICO_PLATFORM
# and PICO_BOARD are set in time.
include(${CMAKE_CURRENT_LIST_DIR}/cmake/picoadk/board.cmake)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(picoadk LANGUAGES C CXX ASM VERSION 3.0.0)

pico_sdk_init()

# FreeRTOS (port-aware).
include(${CMAKE_CURRENT_LIST_DIR}/cmake/picoadk/freertos.cmake)

# Vult helper function.
include(${CMAKE_CURRENT_LIST_DIR}/cmake/picoadk/vult.cmake)

# ---------------------------------------------------------------------------
# picoadk_hal — the static library every app links against.
# ---------------------------------------------------------------------------

add_library(picoadk_hal STATIC
    src/hal/system.cpp
    src/hal/audio.cpp
    src/hal/audio_engine.cpp
    src/hal/controls.cpp
    src/hal/encoders.cpp
    src/hal/buttons.cpp
    src/hal/mux.cpp
    src/hal/storage.cpp
    src/hal/display.cpp
    src/hal/hardware.cpp
    src/midi/midi.cpp
    src/midi/midi_usb.cpp
    src/midi/midi_uart.cpp
    src/midi/general_handler.cpp
    src/midi/launchpad.cpp
    src/usb/usb_descriptors.c
    src/usb/get_serial.c
    src/usb/usb_device.cpp
    src/usb/usb_host.cpp
    src/usb/msc.cpp
    src/dsp/math.cpp
    src/dsp/modules.cpp
    src/dsp/sample_player.cpp
    src/dsp/pd.cpp
    src/dsp/vultin.cpp
    src/hal/voicelink.cpp
)

if(PICOADK_BOARD STREQUAL "v2_rp2350")
    target_sources(picoadk_hal PRIVATE
        src/hal/psram_rp2350.cpp
        src/third_party/tlsf/tlsf.c
    )
else()
    target_sources(picoadk_hal PRIVATE
        src/hal/psram_stub.cpp
    )
endif()

target_include_directories(picoadk_hal PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/boards
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/src/third_party/tlsf
    ${CMAKE_CURRENT_LIST_DIR}/lib/u8g2/csrc
)

# u8g2 — vendored panel driver / fonts / primitives. We compile its C
# sources straight into picoadk_hal so apps don't need to think about it.
file(GLOB _picoadk_u8g2_src "${CMAKE_CURRENT_LIST_DIR}/lib/u8g2/csrc/*.c")
target_sources(picoadk_hal PRIVATE ${_picoadk_u8g2_src})

target_compile_definitions(picoadk_hal PUBLIC
    ${PICOADK_BOARD_DEF}
)

target_link_libraries(picoadk_hal PUBLIC
    pico_stdlib
    pico_unique_id
    pico_multicore
    pico_util_buffer
    pico_stdio_uart
    hardware_adc
    hardware_spi
    hardware_i2c
    hardware_dma
    hardware_pio
    hardware_interp
    hardware_timer
    hardware_watchdog
    hardware_clocks
    hardware_uart
    hardware_vreg
    tinyusb_device
    tinyusb_board
    FreeRTOS
)

# tinyusb compiles its own sources via the tinyusb_device interface library;
# its translation units need to find tusb_config.h in src/usb. Adding the
# include path INTERFACE on tinyusb_common propagates correctly because
# pico-sdk creates that target during pico_sdk_init().
target_include_directories(tinyusb_common INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/usb)

if(PICOADK_BOARD STREQUAL "v2_rp2350")
    target_link_libraries(picoadk_hal PUBLIC hardware_dcp)
    if(PICOADK_ENABLE_SDIO)
        include(${CMAKE_CURRENT_LIST_DIR}/cmake/picoadk/sdio.cmake)
        if(TARGET picoadk_sdio_v2)
            target_link_libraries(picoadk_hal PUBLIC picoadk_sdio_v2)
            target_compile_definitions(picoadk_hal PUBLIC PICOADK_HAS_SDIO_RP2350=1)
        endif()
    endif()
endif()

if(PICOADK_ENABLE_SDFAT)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/picoadk/sdfat.cmake)
    target_link_libraries(picoadk_hal PUBLIC picoadk_sdfat)
    target_compile_definitions(picoadk_hal PUBLIC PICOADK_HAS_SDFAT=1)
endif()

if(PICOADK_LIBPD)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/picoadk/libpd.cmake)
    target_link_libraries(picoadk_hal PUBLIC picoadk_libpd)
    target_compile_definitions(picoadk_hal PUBLIC PICOADK_LIBPD=1)
endif()

# v2 PSRAM linker fragment is applied to the final executable in
# examples/<app>/CMakeLists.txt — the HAL alone doesn't link an executable.
set(PICOADK_PSRAM_LD "${CMAKE_CURRENT_LIST_DIR}/boards/picoadk_v2_psram.ld" CACHE FILEPATH "")

# Generate any board PIO programs into the HAL.
file(GLOB _picoadk_pio "${CMAKE_CURRENT_LIST_DIR}/src/hal/pio/*.pio")
foreach(_p IN LISTS _picoadk_pio)
    pico_generate_pio_header(picoadk_hal ${_p})
endforeach()

# ---------------------------------------------------------------------------
# Application — selectable, defaults to the Vult monosynth.
# ---------------------------------------------------------------------------

if(NOT DEFINED PICOADK_APP)
    set(PICOADK_APP "examples/02_synth_vult" CACHE STRING
        "App directory to build (must contain CMakeLists.txt)")
endif()

message(STATUS "PicoADK app: ${PICOADK_APP}")

if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${PICOADK_APP}/CMakeLists.txt")
    message(FATAL_ERROR
        "App directory '${PICOADK_APP}' has no CMakeLists.txt. "
        "Set -DPICOADK_APP=<dir> to a folder under examples/ or your own app.")
endif()

add_subdirectory(${PICOADK_APP} app)

# Common compile options applied after the app is added so they hit user code too.
add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wredundant-decls)
