# Chip-8 Emulator

A functioning CHIP-8 emulator written in C++20 using SDL3

## Prerequisites
- Xcode Command Line
- CMake (v 4.4.2 was used)
- SDL3 (`brew install sdl3`)
- C++20

## Installing

```zsh
git clone https://github.com/Kv1st3n/Chip-8-Emulator.git
cd chip8
cmake --build build
.build/chip8 <roms>
```

## Example
![Breakout](screenshots/breakout.png)
*breakout*

## Note
This repo does not include any ROMS for Chip8, but they are easy to find by simply searching "Chip8 roms".
Another note is that some ROMS may not function properly (most likely my code), but some Pong roms does not update the scores.

## References
Considering emulation is a new endeavor for me, I used the following resources that helped me with the development. These resources are useful in explaining the functionality and gives great idea of the overall structure:

- https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
- https://austinmorlan.com/posts/chip8_emulator/
- https://github.com/trapexit/chip-8_documentation