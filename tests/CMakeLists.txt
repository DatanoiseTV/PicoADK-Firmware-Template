# Host-side unit tests for the picoadk DSP modules.
#
# Builds with the system compiler (NOT arm-none-eabi-gcc) so the tests can run
# on the developer machine. Useful for catching DSP correctness bugs before
# flashing.
#
# Configure & run:
#     cmake -B build/tests -S tests
#     cmake --build build/tests
#     ctest --test-dir build/tests --output-on-failure

cmake_minimum_required(VERSION 3.20)
project(picoadk_tests LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Stub out PSRAM + Storage for the host build — the DSP modules don't need
# them when used directly. PICOADK_BOARD_SELECT_V2 enables the float Real path.
add_compile_definitions(
    PICOADK_BOARD_SELECT_V2
    PICOADK_BOARD_V1=0
    PICOADK_BOARD_V2=1
    PICOADK_HAS_FPU=1
    PICOADK_HAS_PSRAM=0)

add_library(picoadk_dsp_host STATIC
    ../src/dsp/modules.cpp
    ../src/dsp/math.cpp
    ../src/dsp/sample_player.cpp
    ../src/third_party/tlsf/tlsf.c
    storage_host_stub.cpp
    psram_host_stub.cpp
)
target_include_directories(picoadk_dsp_host PUBLIC
    ../include
    ../boards
    ../src/third_party/tlsf
)

enable_testing()

function(picoadk_add_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE picoadk_dsp_host)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

picoadk_add_test(test_oscillators)
picoadk_add_test(test_filters)
picoadk_add_test(test_envelope)
picoadk_add_test(test_delay)
picoadk_add_test(test_compressor)
picoadk_add_test(test_robustness)
picoadk_add_test(test_sample_player)
picoadk_add_test(test_voicelink_crc)
picoadk_add_test(test_real_q16)
picoadk_add_test(test_tlsf)
