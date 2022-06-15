
add_library(Audio STATIC
	src/audio.cpp
	src/audio_i2s.c
	src/audio_subsystem.cpp
)

target_include_directories(Audio PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
)

pico_generate_pio_header(Audio ${CMAKE_CURRENT_LIST_DIR}/piosrc/audio_i2s.pio)

target_link_libraries(Audio pico_stdlib pico_util_buffer pico_multicore hardware_pio hardware_dma)
