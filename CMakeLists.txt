cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# Add option for using precompiled libraries
option(USE_PRECOMPILED_LIBS "Use precompiled libraries for audio and pico-sdk" OFF)

set(PICO_EXTRAS_PATH "../lib/pico-extras")
set(PICO_BOARD datanoisetv_rp2350_dsp)
set(PICO_PLATFORM "rp2350-arm-s")
set(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeRTOS-Kernel")

if (USE_PRECOMPILED_LIBS)
    set(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/precompiled/pico-sdk")
else()
    set(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/pico-sdk")
endif()

# Define values for the placeholders
set(FLASH_LENGTH 4M)      # Example value for FLASH length
set(PSRAM_LENGTH 8M)      # Example value for PSRAM length
set(RAM_LENGTH 520K)        # Example value for RAM length
set(EEPROM_START 0x1FFF000) # Example EEPROM start address
set(FS_START 0x08080000)    # Example file system start address
set(FS_END 0x080C0000)      # Example file system end address

set(DSP_SAMPLE_RATE 48000)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)
include(FreeRTOS_Kernel_import.cmake)

project(main C CXX ASM)

pico_sdk_init()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR} ".")

if (USE_PRECOMPILED_LIBS)
    add_library(audio STATIC IMPORTED)
    set_target_properties(audio PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/precompiled/audio.a")
else()
    add_subdirectory(lib/audio)
endif()

add_subdirectory(lib/oled)
add_subdirectory(lib/heavy)

# Set the real flag based on the PICO_PLATFORM variable
if (DEFINED PICO_PLATFORM AND PICO_PLATFORM STREQUAL "rp2350-arm-s")
    set(REAL_TYPE_FLAG "float")
    message("Using float for real type")
else()
    set(REAL_TYPE_FLAG "fixed")
    message("Using fixed for real type")
endif()

if (NOT FREERTOS_KERNEL_PATH AND NOT DEFINED ENV{FREERTOS_KERNEL_PATH})
    message("Skipping FreeRTOS examples as FREERTOS_KERNEL_PATH not defined")
    return()
endif()

add_executable(main
    src/usb_descriptors.c
    src/main.cpp 
    src/picoadk_hw.cpp
    src/midi_input_usb.cpp
    src/get_serial.c
    # src/psram.cpp
    lib/FreeRTOS-Kernel/portable/MemMang/heap_3.c
)

pico_set_program_name(main "main")
pico_set_program_version(main "0.1")

pico_enable_stdio_uart(main 1)
pico_enable_stdio_usb(main 0)

# Add the standard library to the build
target_link_libraries(main pico_stdlib)

# Add any user requested libraries
if (USE_PRECOMPILED_LIBS)
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
        hardware_sync_spin_lock
        hardware_sync
        pico_unique_id
        pico_bootsel_via_double_reset
        pico_util_buffer
        pico_multicore
        pico_stdio_uart
        tinyusb_device
        tinyusb_host
        tinyusb_board
        audio
        FreeRTOS-Kernel
        heavy
    )
else()
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
        hardware_sync_spin_lock
        hardware_sync
        pico_unique_id
        pico_bootsel_via_double_reset
        pico_util_buffer
        pico_multicore
        pico_stdio_uart
        tinyusb_device
        tinyusb_host
        tinyusb_board
        Audio
        FreeRTOS-Kernel
        heavy
    )
endif()

target_compile_definitions(main PRIVATE
    USE_AUDIO_I2S=1
    PICO_USE_SW_SPIN_LOCKS=1
    USE_USB_MIDI_HOST=0
    SAMPLE_RATE=${DSP_SAMPLE_RATE}
    RP2350_PSRAM_CS=0
)

target_compile_definitions(Audio PRIVATE
    SAMPLE_RATE=${DSP_SAMPLE_RATE}
)

# # Configure the linker script with actual values
# configure_file(
#     ${CMAKE_CURRENT_SOURCE_DIR}/default.ld.in  # Input template file
#     ${CMAKE_CURRENT_BINARY_DIR}/default.ld     # Output linker script file
#     @ONLY                                      # Use only @var@ style placeholders
# )

# Set the linker script to use the custom default.ld
# pico_set_linker_script(main ${CMAKE_CURRENT_BINARY_DIR}/default.ld)


target_link_options(main PRIVATE
    -Xlinker --print-memory-usage
)

file(GLOB files "${CMAKE_CURRENT_LIST_DIR}/piosrc/*.pio")
foreach(file ${files})
    message ("Adding ${file} to PIO build.")
    pico_generate_pio_header(main ${file})
endforeach()

add_compile_options("-Wall" "-Wredundant-decls")

if (PICO_PLATFORM STREQUAL "rp2040")
    pico_define_boot_stage2(slower_boot2 ${PICO_DEFAULT_BOOT_STAGE2_FILE})
    target_compile_definitions(slower_boot2 PRIVATE PICO_FLASH_SPI_CLKDIV=4)
    pico_set_boot_stage2(main slower_boot2)
else()
    message("Skipping slower_boot2 as PICO_PLATFORM is not rp2040")
endif()

pico_add_extra_outputs(main)
