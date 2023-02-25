
add_library(AudioNew STATIC
src/i2s.c
)

target_include_directories(AudioNew PUBLIC
${CMAKE_CURRENT_LIST_DIR}/include
)

pico_generate_pio_header(AudioNew ${CMAKE_CURRENT_LIST_DIR}/piosrc/i2s.pio)

target_link_libraries(AudioNew pico_stdlib pico_util_buffer pico_multicore hardware_pio hardware_dma)
