
add_library(Oled STATIC
	src/oled.cpp
)

target_include_directories(Oled PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(Oled pico_stdlib hardware_i2c)
target_compile_options(Oled PUBLIC
	-Wno-deprecated-declarations
	-Wno-unused-variable
	-Wno-unused-function
	-Wno-unused-parameter
	-Wno-write-strings
	-Wno-narrowing
)