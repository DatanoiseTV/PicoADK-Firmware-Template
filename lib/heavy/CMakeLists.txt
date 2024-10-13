cmake_minimum_required(VERSION 3.13)

add_library(heavy INTERFACE)
# GLOB all .c and .cpp files in the current directory
file(GLOB HV_SOURCES "*.c" "*.cpp")

# Add the sources to the target
target_sources(heavy INTERFACE ${HV_SOURCES})

target_include_directories(heavy INTERFACE
 ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(heavy INTERFACE)
