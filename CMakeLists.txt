cmake_minimum_required(VERSION 3.12)

# set project name and version
project(chip8 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(chip8 main.cpp)

find_package(SDL3 REQUIRED)

add_subdirectory(src)

target_link_libraries(chip8 PRIVATE Chip8Library SDL3::SDL3)

target_include_directories(chip8 PRIVATE
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src"
) 





