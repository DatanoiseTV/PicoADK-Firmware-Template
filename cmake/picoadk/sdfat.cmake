# SdFat integration (Phase 4).
#
# Pulls SdFat (https://github.com/greiman/SdFat, MIT) at configure time when
# storage support is enabled. Built as a CMake INTERFACE library to skip the
# Arduino-style build wrapper SdFat ships.

include(FetchContent)

FetchContent_Declare(
    sdfat
    GIT_REPOSITORY https://github.com/greiman/SdFat.git
    GIT_TAG        2.2.3
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(sdfat)

add_library(picoadk_sdfat STATIC
    ${sdfat_SOURCE_DIR}/src/SdFat.h
    ${sdfat_SOURCE_DIR}/src/FatLib/FatFile.cpp
    ${sdfat_SOURCE_DIR}/src/FatLib/FatFileLFN.cpp
    ${sdfat_SOURCE_DIR}/src/FatLib/FatFileSFN.cpp
    ${sdfat_SOURCE_DIR}/src/FatLib/FatFormatter.cpp
    ${sdfat_SOURCE_DIR}/src/FatLib/FatPartition.cpp
    ${sdfat_SOURCE_DIR}/src/FatLib/FatVolume.cpp
    ${sdfat_SOURCE_DIR}/src/ExFatLib/ExFatFile.cpp
    ${sdfat_SOURCE_DIR}/src/ExFatLib/ExFatFilePrint.cpp
    ${sdfat_SOURCE_DIR}/src/ExFatLib/ExFatFormatter.cpp
    ${sdfat_SOURCE_DIR}/src/ExFatLib/ExFatPartition.cpp
    ${sdfat_SOURCE_DIR}/src/ExFatLib/ExFatVolume.cpp
    ${sdfat_SOURCE_DIR}/src/common/FmtNumber.cpp
    ${sdfat_SOURCE_DIR}/src/common/FsCache.cpp
    ${sdfat_SOURCE_DIR}/src/common/FsName.cpp
    ${sdfat_SOURCE_DIR}/src/common/FsStructs.cpp
    ${sdfat_SOURCE_DIR}/src/common/FsUtf.cpp
    ${sdfat_SOURCE_DIR}/src/common/upcase.cpp
    ${sdfat_SOURCE_DIR}/src/SpiDriver/SdSpiDriver.cpp
)
target_include_directories(picoadk_sdfat PUBLIC ${sdfat_SOURCE_DIR}/src)
target_compile_definitions(picoadk_sdfat PUBLIC
    USE_LONG_FILE_NAMES=1
    USE_UTF8_LONG_NAMES=1
    USE_FCNTL_H=0
    SDFAT_FILE_TYPE=3   # FAT16/32 + exFAT
    DISABLE_FS_H_WARNING=1
)
