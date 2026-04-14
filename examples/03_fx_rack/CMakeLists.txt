# FX rack — duplex audio with reverb + delay, modulated by pots.

add_executable(picoadk_app main.cpp)
target_link_libraries(picoadk_app PRIVATE picoadk_hal)

pico_set_program_name(picoadk_app "picoadk_fx_rack")
pico_set_program_version(picoadk_app "3.0.0")
pico_enable_stdio_uart(picoadk_app 1)
pico_enable_stdio_usb(picoadk_app 0)

pico_add_extra_outputs(picoadk_app)
