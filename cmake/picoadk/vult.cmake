# Vult DSP transpiler integration.
#
# Picks the host-appropriate vultc binary, runs it on a .vult source, and emits
# a C++ file that gets linked into the firmware.
#
# On RP2040 we generate Q16 fixed-point code; on RP2350 we generate float code
# so the M33 FPU is actually used. The Vult `-real` flag controls this.
#
# Usage from the top-level CMakeLists:
#     picoadk_add_vult_dsp(<target> SOURCE <file.vult> [INCLUDES <dir>...])

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(_picoadk_vultc "${CMAKE_SOURCE_DIR}/tools/vultc-mac")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(_picoadk_vultc "${CMAKE_SOURCE_DIR}/tools/vultc.exe")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)")
    set(_picoadk_vultc "${CMAKE_SOURCE_DIR}/tools/vultc-arm")
else()
    set(_picoadk_vultc "${CMAKE_SOURCE_DIR}/tools/vultc")
endif()

if(NOT EXISTS "${_picoadk_vultc}")
    message(WARNING "vultc binary not found at ${_picoadk_vultc} — Vult DSP targets will fail to build.")
endif()

set(PICOADK_VULTC_BIN "${_picoadk_vultc}" CACHE FILEPATH "Vult compiler binary")

if(PICOADK_BOARD_V2)
    set(PICOADK_VULT_REAL "float" CACHE STRING "Vult numeric type (float|fixed)")
else()
    set(PICOADK_VULT_REAL "fixed" CACHE STRING "Vult numeric type (float|fixed)")
endif()

set(PICOADK_VULT_SAMPLERATE "48000" CACHE STRING "Sample rate baked into Vult tables")

function(picoadk_add_vult_dsp TARGET)
    cmake_parse_arguments(VULT "" "SOURCE;OUTPUT_BASENAME" "INCLUDES" ${ARGN})
    if(NOT VULT_SOURCE)
        message(FATAL_ERROR "picoadk_add_vult_dsp: SOURCE is required")
    endif()
    if(NOT VULT_OUTPUT_BASENAME)
        set(VULT_OUTPUT_BASENAME "vult")
    endif()

    set(_out "${CMAKE_CURRENT_BINARY_DIR}/${VULT_OUTPUT_BASENAME}.cpp")
    set(_inc_args "")
    foreach(_inc ${VULT_INCLUDES})
        list(APPEND _inc_args "-i" "${_inc}")
    endforeach()

    add_custom_command(
        OUTPUT "${_out}" "${CMAKE_CURRENT_BINARY_DIR}/${VULT_OUTPUT_BASENAME}.h"
        COMMAND "${PICOADK_VULTC_BIN}"
                "${VULT_SOURCE}"
                ${_inc_args}
                -ccode
                -real ${PICOADK_VULT_REAL}
                -samplerate ${PICOADK_VULT_SAMPLERATE}
                -o ${VULT_OUTPUT_BASENAME}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        DEPENDS "${VULT_SOURCE}"
        COMMENT "Vult: ${VULT_SOURCE} -> ${VULT_OUTPUT_BASENAME}.cpp (${PICOADK_VULT_REAL} @ ${PICOADK_VULT_SAMPLERATE} Hz)"
        VERBATIM
    )

    target_sources(${TARGET} PRIVATE "${_out}")
    target_include_directories(${TARGET} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
endfunction()
