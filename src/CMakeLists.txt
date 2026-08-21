add_library(Chip8Library STATIC
    chip8.cpp
    chip8.h
    renderer.h
    renderer.cpp
)

target_include_directories(Chip8Library PUBLIC .)
target_link_libraries(Chip8Library PUBLIC SDL3::SDL3)