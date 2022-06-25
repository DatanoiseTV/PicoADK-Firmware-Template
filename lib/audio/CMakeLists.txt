
add_library(Audio STATIC
<<<<<<< HEAD
=======
	src/audio.cpp
	src/audio_i2s.c
>>>>>>> parent of 1d8afbc (Modify audio lib to use pico-extras.)
	src/audio_subsystem.cpp
)

target_include_directories(Audio PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
)

<<<<<<< HEAD
target_link_libraries(Audio pico_stdlib pico_util_buffer pico_multicore hardware_pio hardware_dma pico_audio pico_audio_i2s)
=======
pico_generate_pio_header(Audio ${CMAKE_CURRENT_LIST_DIR}/piosrc/audio_i2s.pio)

target_link_libraries(Audio pico_stdlib pico_util_buffer pico_multicore hardware_pio hardware_dma)
>>>>>>> parent of 1d8afbc (Modify audio lib to use pico-extras.)
