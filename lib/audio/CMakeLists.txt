
add_library(Audio STATIC
	src/audio_subsystem.cpp
)

target_include_directories(Audio PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(Audio pico_stdlib pico_util_buffer pico_multicore hardware_pio hardware_dma pico_audio pico_audio_i2s)
