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

void Chip8::start_chip8() {
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

void Chip8::run_cycle() {

    start_chip8();

    opcode = (memory[PC] << 8) | memory[PC + 1];

    // decode / execute

    if (delay_timmer > 0) {
        --delay_timmer;
    }

    if (sound_timer > 0) {
        --sound_timer;
    }

}

void Chip8::IN_0NNN() {

}

void Chip8::IN_00E0() {

    memset(display, 0, sizeof(display));
    PC+=2;
}

void Chip8::IN_00EE() {
    --stack_pointer;
    PC = stack[stack_pointer];
    PC+=2;
}

void Chip8::IN_1NNN() {
    PC = opcode & 0x0FFFu;
}

void Chip8::IN_2NNN() {
    stack[stack_pointer] = PC;
    ++stack_pointer;
    PC = opcode & 0x0FFFu;
}

void Chip8::IN_3XNN() {

    u8 NN = (opcode & 0x00FFu);
    u8 VX = (opcode & 0x0F00u) >> 8u;

    if (V[VX] == NN) {
        PC += 4;
    } else {
        PC += 2;
    }
}

void Chip8::IN_4XNN() {

    u8 NN = (opcode & 0x00FFu);
    u8 VX = (opcode & 0x0F00u) >> 8u;

    if (V[VX] != NN) {
        PC += 4;
    } else {
        PC += 2;
    }
}

void Chip8::IN_5XY0() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    if (V[VX] == V[VY]) {
        PC += 4;
    } else {
        PC += 2;
    }
}

void Chip8::IN_6XNN() {

    u8 NN = (opcode & 0x00FFu);
    u8 VX = (opcode & 0x0F00u) >> 8u;

    V[VX] = NN;
    PC += 2;
}

void Chip8::IN_7XNN() {

    u8 NN = (opcode & 0x00FFu);
    u8 VX = (opcode & 0x0F00u) >> 8u;

    V[VX] += NN;
    PC += 2;
}

void Chip8::IN_8XY0() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    V[VX] = V[VY];
    PC += 2;
}

void Chip8::IN_8XY1() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    V[VX] |= V[VY];
    PC += 2;
}

void Chip8::IN_8XY2() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    V[VX] &= V[VY];
    PC += 2;
}

void Chip8::IN_8XY3() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    V[VX] ^= V[VY];
    PC += 2;
}

void Chip8::IN_8XY4() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;
    
    u16 sum = V[VX] + V[VY];

    if (sum > 255U) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[VX] = sum & 0xFFu;
    PC += 2;
}

void Chip8::IN_8XY5() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    if (V[VX] > V[VY]) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[VX] -= V[VY];
    PC += 2;
}

void Chip8::IN_8XY6() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    V[VX] = (V[VX] & 0x1u);
    V[VX] >>= 1;
    PC += 2;

}

void Chip8::IN_8XY7() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    if (V[VY] > V[VX]) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[VY] = V[VY] - V[VX];
    PC += 2;
}

void Chip8::IN_8XYE() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    V[VX] = (V[VX] & 0x1u) >> 7u;
    V[VX] <<= 1;
    PC += 2;
}

void Chip8::IN_9XY0() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    if (V[VX] != V[VY]) {
        PC += 2;
    } else {
        PC += 2;
    }
}

void Chip8::IN_ANNN() {

    u8 address = (opcode & 0x0FFF);
    I = address;
    PC += 2;
}

void Chip8::IN_BNNN() {

    u8 address = (opcode & 0x0FFF);
    PC = V[0] + address;

}

void Chip8::IN_CXNN() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 NN = (opcode & 0x00FFu);

    u8 random_number = (rand() % (0xFF + 1));

    V[VX] = NN & random_number;
    
}

void Chip8::IN_DXYN() {

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;
    u8 Xpos = (VX % 63); // display width
    u8 Ypos = (VY & 31);
    u8 sprite_height = opcode & 0x000Fu;
    u8 pixel;
    V[0xF] = 0;

    for (long row = 0; row < sprite_height; row++) {
        
        pixel = memory[I + row];

        for (long column = 0; column < 8; column++) {

            u8 sprite_pixel = (pixel & (0x80u >> row));
            u16 screen = display[(Xpos + row + ((Ypos + column) * 64))];

            if (sprite_pixel == 1) {

                if (screen == 1) {

                    V[0xF] = 1;
                }
                screen ^= 1;
            }
        }
    }
}






