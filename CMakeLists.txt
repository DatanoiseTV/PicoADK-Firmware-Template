# PicoADK CMakeLists.txt

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(PICO_EXTRAS_PATH "../lib/pico-extras")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

set(PICO_BOARD datanoisetv_rp2040_dsp)

set(FREERTOS_SRC_DIRECTORY "${PICO_SDK_PATH}/lib/tinyusb/lib/FreeRTOS-Kernel")

add_library(FreeRTOS STATIC
    ${FREERTOS_SRC_DIRECTORY}/event_groups.c
    ${FREERTOS_SRC_DIRECTORY}/list.c
    ${FREERTOS_SRC_DIRECTORY}/queue.c
    ${FREERTOS_SRC_DIRECTORY}/stream_buffer.c
    ${FREERTOS_SRC_DIRECTORY}/tasks.c
    ${FREERTOS_SRC_DIRECTORY}/timers.c
    ${FREERTOS_SRC_DIRECTORY}/portable/MemMang/heap_3.c
    ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0/port.c
)

target_include_directories(FreeRTOS PUBLIC
    ${FREERTOS_CFG_DIRECTORY}/
    ${FREERTOS_SRC_DIRECTORY}/include
    ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0
)


project(main C CXX ASM)

pico_sdk_init()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR} ".")

add_subdirectory(lib/audio)
add_subdirectory(lib/oled)

message("CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message("CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")

# Generate Vult C++ code from Vult Code every time
set(VULT_EXAMPLE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/vult/examples")
add_custom_command(
        OUTPUT ALL ${CMAKE_CURRENT_BINARY_DIR}/vult.cpp
        COMMAND vultc ${CMAKE_CURRENT_LIST_DIR}/vultsrc/dsp.vult
                -i ${VULT_EXAMPLE_PATH}/osc
                -i ${VULT_EXAMPLE_PATH}/util
                -i ${VULT_EXAMPLE_PATH}/filters
                -i ${VULT_EXAMPLE_PATH}/env
                -i ${VULT_EXAMPLE_PATH}/midi
                -i ${VULT_EXAMPLE_PATH}/effects
                -i ${VULT_EXAMPLE_PATH}/units
                -ccode
                -real fixed
                -samplerate 44140
                -o vult 
        COMMENT "Transcompiling Vult DSP Code to C++."
)

add_executable(main
    vultsrc/dsp.vult
    src/usb_descriptors.c
    src/main.cpp 
    src/picoadk_hw.cpp
    src/vultin.cpp
    src/midi_input_usb.cpp
    src/get_serial.c
    ${CMAKE_CURRENT_BINARY_DIR}/vult.cpp)

pico_set_program_name(main "main")
pico_set_program_version(main "0.1")

pico_enable_stdio_uart(main 1)
pico_enable_stdio_usb(main 0)

# Add the standard library to the build
target_link_libraries(main pico_stdlib)

# Add any user requested libraries
target_link_libraries(main
        hardware_adc
        hardware_spi
        hardware_i2c
        hardware_dma
        hardware_pio
        hardware_interp
        hardware_timer
        hardware_watchdog
        hardware_clocks
        pico_unique_id
        pico_bootsel_via_double_reset
        pico_util_buffer
        pico_multicore
        pico_stdio_uart
        tinyusb_device
        tinyusb_board
	Audio
        #Oled
        FreeRTOS
        )

target_compile_definitions(main PRIVATE
        USE_AUDIO_I2S=1
)

target_link_options(main PRIVATE
        -Xlinker --print-memory-usage
)

file(GLOB files "${CMAKE_CURRENT_LIST_DIR}/piosrc/*.pio")
foreach(file ${files})
    message ("Adding ${file} to PIO build.")
    pico_generate_pio_header(main ${file})
endforeach()

add_compile_options("-Wall" "-Wredundant-decls")

pico_define_boot_stage2(slower_boot2 ${PICO_DEFAULT_BOOT_STAGE2_FILE})
target_compile_definitions(slower_boot2 PRIVATE PICO_FLASH_SPI_CLKDIV=4)

pico_set_boot_stage2(main slower_boot2)

pico_add_extra_outputs(main)
