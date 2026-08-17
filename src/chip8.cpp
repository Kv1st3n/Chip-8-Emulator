#include "chip8.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <random>

unsigned char chip8_font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const unsigned int START_ADDRESS = 0x200;
const unsigned int MEMORY_SIZE = 0x1000; // 4Kb (4096)

Chip8::Chip8() {}
Chip8::~Chip8() {}

void Chip8::clear_stack(u16 stack, u8 V, u8 keypad) {

    for (int i = 0; i < 16; i++) {
        this->stack[i] = 0;
        this->V[i] = 0;
        this->keypad[i] = 0;
    }

}

void Chip8::clear_display(u8 display) {

    for (int i = 0; i < 2048; i++) {
        this->display[i] = 0;
    }
}

void Chip8::clear_memory(u8 memory) {

    for (int i = 0; i < 4096; i++) {
        this->memory[i] = 0;
    }
    
}

void Chip8::load_font(u8 memory) {

    for (int i = 0; i < 4096; i++) {
        this->memory[i] = chip8_font[i];
    }

}

void Chip8::init() {
    PC = START_ADDRESS; // starting adress
    I = 0;
    opcode = 0;
    stack_pointer = 0;

    clear_stack(*stack, *V, *keypad);
    clear_display(*display);
    clear_memory(*memory);
    load_font(*memory);

}

bool Chip8::load_game_rom(const char *file_path) {

    std::ifstream file(file_path, std::ios::binary);

    if (file.is_open()) {

        // get length of rom
        file.seekg(0, file.end);
        long rom_size = file.tellg();
        file.seekg(0, file.beg);

        // allocate buffer based on size of rome
        char* buffer = new char[rom_size];

        file.read(buffer, rom_size);
        file.close();

        // load to memory
        // check if it can be loaded
        if ((MEMORY_SIZE - START_ADDRESS) > rom_size) {
            for (long i = 0; i < rom_size; i++) {
                memory[i + START_ADDRESS] = buffer[i];
            }
        } else {
            std::cerr << "ROM is to large" << std::endl;
            return false;
        }
        // free the buffer
        delete[] buffer;
        return true;

    } else {
        std::cerr << "ROM can not be opened or found" << std::endl;
        return false;
    }
}