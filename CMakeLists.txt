# PicoADK CMakeLists.txt

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(PICO_EXTRAS_PATH "../lib/pico-extras")
set(PICO_BOARD datanoisetv_rp2350_dsp)
set(PICO_PLATFORM "rp2350-arm-s")
set(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeRTOS-Kernel")
set(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/pico-sdk")


# Define values for the placeholders
set(FLASH_LENGTH 4M)      # Example value for FLASH length
set(PSRAM_LENGTH 8M)      # Example value for PSRAM length
set(RAM_LENGTH 520K)        # Example value for RAM length
set(EEPROM_START 0x1FFF000) # Example EEPROM start address
set(FS_START 0x08080000)    # Example file system start address
set(FS_END 0x080C0000)      # Example file system end address


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)
include(FreeRTOS_Kernel_import.cmake)

project(main C CXX ASM)

pico_sdk_init()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR} ".")

add_subdirectory(lib/audio)
add_subdirectory(lib/oled)

# Set the real flag based on the PICO_PLATFORM variable
if (DEFINED PICO_PLATFORM AND PICO_PLATFORM STREQUAL "rp2350-arm-s")
    set(REAL_TYPE_FLAG "float")
    message("Using float for real type")
else()
    set(REAL_TYPE_FLAG "fixed")
    message("Using fixed for real type")
endif()



# You could add your DSP transcompiler here
# set(VULT_EXAMPLE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/vult/examples")
# add_custom_command(
#    OUTPUT ALL ${CMAKE_CURRENT_BINARY_DIR}/vult.cpp
#    COMMAND vultc ${CMAKE_CURRENT_LIST_DIR}/vultsrc/dsp.vult
#            -i ${VULT_EXAMPLE_PATH}/osc
#            -i ${VULT_EXAMPLE_PATH}/util
#            -i ${VULT_EXAMPLE_PATH}/filters
#            -i ${VULT_EXAMPLE_PATH}/env
#            -i ${VULT_EXAMPLE_PATH}/midi
#            -i ${VULT_EXAMPLE_PATH}/effects
#            -i ${VULT_EXAMPLE_PATH}/units
#            -ccode
#            -real ${REAL_TYPE_FLAG}
#            -samplerate 44100
#            -o vult 
#    COMMENT "Transcompiling Vult DSP Code to C++."
#)

# TODO: PSRAM Linker Map
#configure_file(
#    ${CMAKE_SOURCE_DIR}/memmap.ld.in  # Input template file
#    ${CMAKE_BINARY_DIR}/memmap.ld     # Output linker script file
#    @ONLY                             # Use only @var@ style placeholders
#)

if (NOT FREERTOS_KERNEL_PATH AND NOT DEFINED ENV{FREERTOS_KERNEL_PATH})
    message("Skipping FreeRTOS examples as FREERTOS_KERNEL_PATH not defined")
    return()
endif()

add_executable(main
    # vultsrc/dsp.vult
    src/usb_descriptors.c
    src/main.cpp 
    src/picoadk_hw.cpp
    src/midi_input_usb.cpp
    src/get_serial.c
    lib/FreeRTOS-Kernel/portable/MemMang/heap_3.c
)

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
        #Oled
        FreeRTOS-Kernel
        )

target_compile_definitions(main PRIVATE
        USE_AUDIO_I2S=1
        PICO_USE_SW_SPIN_LOCKS=1
)

target_link_options(main PRIVATE
        -Xlinker --print-memory-usage
)

# TODO: PSRAM Linker Map
# pico_set_linker_script(main ${CMAKE_BINARY_DIR}/memmap.ld)

file(GLOB files "${CMAKE_CURRENT_LIST_DIR}/piosrc/*.pio")
foreach(file ${files})
    message ("Adding ${file} to PIO build.")
    pico_generate_pio_header(main ${file})
endforeach()

add_compile_options("-Wall" "-Wredundant-decls")

# compile the following thins only if rp2040 is used
if (PICO_PLATFORM STREQUAL "rp2040")
pico_define_boot_stage2(slower_boot2 ${PICO_DEFAULT_BOOT_STAGE2_FILE})
target_compile_definitions(slower_boot2 PRIVATE PICO_FLASH_SPI_CLKDIV=4)

pico_set_boot_stage2(main slower_boot2)
else()
message("Skipping slower_boot2 as PICO_PLATFORM is not rp2040")
endif()

pico_add_extra_outputs(main)
