# FreeRTOS kernel — using the upstream FreeRTOS-Kernel submodule.
#
# Raspberry Pi's recommended path:
#   * RP2040  — `portable/ThirdParty/GCC/RP2040/` (SMP-capable Pico port shipped
#               with the upstream FreeRTOS-Kernel repo, integrated via
#               `FreeRTOS_Kernel_import.cmake`).
#   * RP2350  — `portable/GCC/ARM_CM33_NTZ/non_secure/` (the standard CM33
#               non-TrustZone port).
#
# Both come from the same `lib/FreeRTOS-Kernel` submodule (pinned to V11.3.0)
# so applications never have to think about which kernel they're linking.

if(NOT DEFINED FREERTOS_KERNEL_PATH)
    set(FREERTOS_KERNEL_PATH "${CMAKE_SOURCE_DIR}/lib/FreeRTOS-Kernel" CACHE PATH "")
endif()

if(NOT EXISTS "${FREERTOS_KERNEL_PATH}/tasks.c")
    message(FATAL_ERROR
        "FreeRTOS-Kernel not found at ${FREERTOS_KERNEL_PATH}. "
        "Run: git submodule update --init --recursive")
endif()

# Where the FreeRTOSConfig.h lives — used by both port flavours.
set(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_SOURCE_DIR}/include" CACHE PATH "")

if(PICOADK_BOARD STREQUAL "v1_rp2040")
    # The RPi-supported port ships its own import helper that wires up the
    # SMP Pico port and exposes FreeRTOS-Kernel + FreeRTOS-Kernel-Heap4
    # interface targets.
    include("${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake")
    add_library(picoadk_freertos INTERFACE)
    target_link_libraries(picoadk_freertos INTERFACE
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4)
    message(STATUS "FreeRTOS port: ThirdParty/GCC/RP2040 (SMP, V11.3.0)")
else()
    # RP2350 — Cortex-M33 non-TrustZone. We assemble the targets manually so
    # we don't need a board-specific port file.
    set(_freertos_port "${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM33_NTZ/non_secure")

    add_library(picoadk_freertos_kernel STATIC
        ${FREERTOS_KERNEL_PATH}/croutine.c
        ${FREERTOS_KERNEL_PATH}/event_groups.c
        ${FREERTOS_KERNEL_PATH}/list.c
        ${FREERTOS_KERNEL_PATH}/queue.c
        ${FREERTOS_KERNEL_PATH}/stream_buffer.c
        ${FREERTOS_KERNEL_PATH}/tasks.c
        ${FREERTOS_KERNEL_PATH}/timers.c
        ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c
        ${_freertos_port}/port.c
        ${_freertos_port}/portasm.c
    )
    target_include_directories(picoadk_freertos_kernel PUBLIC
        ${FREERTOS_KERNEL_PATH}/include
        ${_freertos_port}
        ${FREERTOS_CONFIG_FILE_DIRECTORY})
    target_link_libraries(picoadk_freertos_kernel PUBLIC pico_base_headers)

    add_library(picoadk_freertos INTERFACE)
    target_link_libraries(picoadk_freertos INTERFACE picoadk_freertos_kernel)
    message(STATUS "FreeRTOS port: GCC/ARM_CM33_NTZ/non_secure (V11.3.0)")
endif()

# Friendly alias used by the rest of the build.
add_library(FreeRTOS ALIAS picoadk_freertos)
