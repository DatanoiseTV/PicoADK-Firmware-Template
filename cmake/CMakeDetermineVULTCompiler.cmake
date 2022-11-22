# Find the compiler
find_program(
    CMAKE_VULT_COMPILER 
        NAMES "vultc"
        HINTS "${CMAKE_SOURCE_DIR}"
        DOC "VULT DSP compiler" 
)

mark_as_advanced( CMAKE_VULT_COMPILER )

set( CMAKE_VULT_SOURCE_FILE_EXTENSIONS vult )
# Remember this as a potential error
set( CMAKE_VULT_OUTPUT_EXTENSION .cpp )
set( CMAKE_VULT_COMPILER_ENV_VAR "" )

# Configure variables set in this file for fast reload later on
configure_file( ${CMAKE_CURRENT_LIST_DIR}/CMakeVULTCompiler.cmake.in
                ${CMAKE_PLATFORM_INFO_DIR}/CMakeVULTCompiler.cmake )
