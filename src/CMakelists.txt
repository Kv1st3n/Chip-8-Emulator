add_library(Chip8Library STATIC
    chip8.cpp
    chip8.h
)

target_include_directories(Chip8Library PUBLIC .)

