# Generate the C files
set( CMAKE_VULT_COMPILE_OBJECT
    "<CMAKE_VULT_COMPILER> <SOURCE> -ccode -real fixed -samplerate 44100 -o <TARGET>"
)

# Build a executable 
set( CMAKE_VULT_LINK_EXECUTABLE 
    "<CMAKE_VULT_COMPILER> -o <TARGET> -exe <OBJECTS>"
)

set( CMAKE_VULT_INFORMATION_LOADED 1 )
