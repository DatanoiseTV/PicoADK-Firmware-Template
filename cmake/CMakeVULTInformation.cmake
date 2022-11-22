# Generate the C files
set( VULT_EXAMPLE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/vult/examples" )
set( CMAKE_VULT_COMPILE_OBJECT "<CMAKE_VULT_COMPILER> <SOURCE> -i ${VULT_EXAMPLE_PATH}/osc -i ${VULT_EXAMPLE_PATH}/util -i ${VULT_EXAMPLE_PATH}/filters -i ${VULT_EXAMPLE_PATH}/env -i ${VULT_EXAMPLE_PATH}/midi -i ${VULT_EXAMPLE_PATH}/effects -i ${VULT_EXAMPLE_PATH}/units -ccode -real fixed -samplerate 44140 -o vult")
set( CMAKE_VULT_OUTPUT_EXTENSION ".cpp")


set( CMAKE_VULT_INFORMATION_LOADED 1 )
