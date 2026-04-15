# libpd integration (Phase 6).
#
# Pulls libpd at configure time when -DPICOADK_LIBPD=ON. libpd's CMake build
# expects ALSA/CoreAudio etc. so we vendor only the source files we need
# (the audio engine itself, with no host I/O wrappers) and compile them
# directly into picoadk_hal.

include(FetchContent)

FetchContent_Declare(
    libpd
    GIT_REPOSITORY https://github.com/libpd/libpd.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(libpd)

file(GLOB LIBPD_SRC
    ${libpd_SOURCE_DIR}/pure-data/src/*.c
    ${libpd_SOURCE_DIR}/libpd_wrapper/*.c
)
list(FILTER LIBPD_SRC EXCLUDE REGEX ".*s_(audio|midi)_(alsa|jack|portaudio|pa|pm|dummy).*\\.c$")
list(FILTER LIBPD_SRC EXCLUDE REGEX ".*s_main\\.c$")

add_library(picoadk_libpd STATIC ${LIBPD_SRC})
target_include_directories(picoadk_libpd PUBLIC
    ${libpd_SOURCE_DIR}/pure-data/src
    ${libpd_SOURCE_DIR}/libpd_wrapper
)
target_compile_definitions(picoadk_libpd PUBLIC
    PD             # tells the source it's the embedded build
    LIBPD_USE_STD_MATH=1
    HAVE_UNISTD_H=0
)
