add_library(Chip8Library STATIC
    chip8.cpp
    chip8.h
    renderer.h
    renderer.cpp
)

target_include_directories(Chip8Library PUBLIC .)

