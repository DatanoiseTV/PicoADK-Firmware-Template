# FreeRTOS kernel — Raspberry Pi's SMP fork.
#
# `lib/FreeRTOS-Kernel` is `raspberrypi/FreeRTOS-Kernel` (FreeRTOS V11 with
# RPi-curated SMP ports for both RP2040 and RP2350). This gives us:
#
#   * configNUMBER_OF_CORES = 2 on both boards.
#   * configUSE_CORE_AFFINITY → xTaskCreatePinnedToCore Just Works.
#   * pico_sync (mutex/sem/queue) interop with non-FreeRTOS code on the
#     opposite core.
#
# Each port ships its own `FreeRTOS_Kernel_import.cmake` that declares the
# standard `FreeRTOS-Kernel` and `FreeRTOS-Kernel-Heap[1..5]` INTERFACE
# targets, so we just include the right one per board.

if(NOT DEFINED FREERTOS_KERNEL_PATH)
    set(FREERTOS_KERNEL_PATH "${CMAKE_SOURCE_DIR}/lib/FreeRTOS-Kernel" CACHE PATH "")
endif()

if(NOT EXISTS "${FREERTOS_KERNEL_PATH}/tasks.c")
    message(FATAL_ERROR
        "FreeRTOS-Kernel not found at ${FREERTOS_KERNEL_PATH}. "
        "Run: git submodule update --init --recursive")
endif()

# FreeRTOSConfig.h lives in include/.
set(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_SOURCE_DIR}/include" CACHE PATH "")

if(PICOADK_BOARD STREQUAL "v1_rp2040")
    include("${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake")
    message(STATUS "FreeRTOS port: ThirdParty/GCC/RP2040 (SMP)")
else()
    include("${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2350_ARM_NTZ/FreeRTOS_Kernel_import.cmake")
    message(STATUS "FreeRTOS port: ThirdParty/GCC/RP2350_ARM_NTZ (SMP)")
endif()

# Friendly alias used by the rest of the build.
add_library(picoadk_freertos INTERFACE)
target_link_libraries(picoadk_freertos INTERFACE
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4)

# Make sure the kernel's compilation can find boards/picoadk_board.h (referenced
# by FreeRTOSConfig.h) and gets the board-selector macro.
target_include_directories(FreeRTOS-Kernel INTERFACE
    ${CMAKE_SOURCE_DIR}/boards)
target_compile_definitions(FreeRTOS-Kernel INTERFACE ${PICOADK_BOARD_DEF})

add_library(FreeRTOS ALIAS picoadk_freertos)
