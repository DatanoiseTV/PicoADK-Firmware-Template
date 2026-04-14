# Example app: the original PicoADK Vult monosynth, ported to the v3 HAL.
#
# Configure the firmware build with -DPICOADK_APP=examples/02_synth_vult
# (this is the default).

add_executable(picoadk_app
    main.cpp
)

target_link_libraries(picoadk_app PRIVATE picoadk_hal)

picoadk_add_vult_dsp(picoadk_app
    SOURCE   ${CMAKE_SOURCE_DIR}/vultsrc/dsp.vult
    INCLUDES ${CMAKE_SOURCE_DIR}/lib/vult/examples/osc
             ${CMAKE_SOURCE_DIR}/lib/vult/examples/util
             ${CMAKE_SOURCE_DIR}/lib/vult/examples/filters
             ${CMAKE_SOURCE_DIR}/lib/vult/examples/env
             ${CMAKE_SOURCE_DIR}/lib/vult/examples/midi
             ${CMAKE_SOURCE_DIR}/lib/vult/examples/effects
             ${CMAKE_SOURCE_DIR}/lib/vult/examples/units
)

pico_set_program_name(picoadk_app "picoadk_synth_vult")
pico_set_program_version(picoadk_app "3.0.0")
pico_enable_stdio_uart(picoadk_app 1)
pico_enable_stdio_usb(picoadk_app 0)

pico_add_extra_outputs(picoadk_app)
