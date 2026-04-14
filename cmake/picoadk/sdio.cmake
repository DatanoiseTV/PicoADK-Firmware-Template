# SDIO_RP2350 (rabbitholecomputing) — RP2350 4-bit SDIO driver.
#
# Pulled at configure time when the v2 board is selected. Provides a clean
# sdio_init / sdio_read / sdio_write API that we plumb into the Storage HAL.
# This avoids reinventing the PIO + DMA + CRC machinery for a high-speed
# SDIO bus on RP2350.
#
# Source: https://github.com/rabbitholecomputing/SDIO_RP2350

include(FetchContent)

FetchContent_Declare(
    sdio_rp2350
    GIT_REPOSITORY https://github.com/rabbitholecomputing/SDIO_RP2350.git
    GIT_TAG        main
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(sdio_rp2350)

# The repo ships its own CMakeLists; expose its target name through a stable
# alias so storage.cpp can just link picoadk_sdio_v2 regardless of upstream
# renames.
if(TARGET sdio_rp2350)
    add_library(picoadk_sdio_v2 ALIAS sdio_rp2350)
elseif(TARGET SDIO_RP2350)
    add_library(picoadk_sdio_v2 ALIAS SDIO_RP2350)
endif()
