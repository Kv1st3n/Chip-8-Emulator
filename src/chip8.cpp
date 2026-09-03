#include "chip8.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <random>

unsigned char chip8_font[80] = 
{
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

Chip8::Chip8() 
{   
    function_pointer();
}

Chip8::~Chip8() {}

void Chip8::clear_stack(u16 &stack, u8 &V, u8 &keypad) 
{

    for (int i = 0; i < 16; i++) 
    {
        this->stack[i] = 0;
        this->V[i] = 0;
        this->keypad[i] = 0;
    }

}

void Chip8::clear_display(u8 &display) 
{

    for (int i = 0; i < 2048; i++) 
    {
        this->display[i] = 0;
    }
}

void Chip8::clear_memory(u8 &memory) 
{

    for (int i = 0; i < 4096; i++) 
    {
        this->memory[i] = 0;
    }
    
}

void Chip8::load_font(u8 &memory)
{

    for (int i = 0; i < 80; i++) 
    {
        this->memory[0x50 + i] = chip8_font[i];
    }

}

void Chip8::start_chip8() 
{
    PC = START_ADDRESS; // starting adress
    I = 0;
    opcode = 0;
    stack_pointer = 0;

    clear_stack(*stack, *V, *keypad);
    clear_display(*display);
    clear_memory(*memory);
    load_font(*memory);

}

bool Chip8::load_game_rom(const char *file_path) 
{

    std::ifstream file(file_path, std::ifstream::binary | std::ifstream::ate);

    if (file.is_open()) 
    {

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
        if ((MEMORY_SIZE - START_ADDRESS) > rom_size) 
        {
            for (long i = 0; i < rom_size; ++i) 
            {
                memory[i + START_ADDRESS] = buffer[i];
            }
        } 
        else 
        {
            std::cerr << "ROM is to large" << std::endl;
            return false;
        }
        // free the buffer
        delete[] buffer;
        return true;

    } 
    else 
    {
        std::cerr << "ROM can not be opened or found" << std::endl;
        return false;
    }
}

void Chip8::run_cycle() 
{

    opcode = (memory[PC] << 8) | memory[PC + 1];

    // decode / execute
    ((*this).*(table[(opcode & 0xF000u) >> 12U]))();
    printf("PC: %04X opcode: %04X\n", PC, opcode);

    if (delay_timer > 0) 
    {
        --delay_timer;
    }

    if (sound_timer > 0) 
    {
        --sound_timer;
    }

}

void Chip8::IN_0NNN() 
{
}

void Chip8::IN_00E0() 
{

    memset(display, 0, sizeof(display));
    PC+=2;
}

void Chip8::IN_00EE() 
{
    --stack_pointer;
    PC = stack[stack_pointer];
    PC+=2;
}

void Chip8::IN_1NNN() 
{
    PC = opcode & 0x0FFFu;
}

void Chip8::IN_2NNN() 
{
    stack[stack_pointer] = PC;
    ++stack_pointer;
    PC = opcode & 0x0FFFu;
}

void Chip8::IN_3XNN() 
{

    u8 NN = (opcode & 0x00FFu);
    u8 VX = (opcode & 0x0F00u) >> 8u;

    if (V[VX] == NN) 
    {
        PC += 4;
    } 
    else 
    {
        PC += 2;
    }
}

void Chip8::IN_4XNN() 
{

    u8 NN = (opcode & 0x00FFu);
    u8 VX = (opcode & 0x0F00u) >> 8u;

    if (V[VX] != NN) 
    {
        PC += 4;
    } 
    else 
    {
        PC += 2;
    }
}

void Chip8::IN_5XY0() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    if (V[VX] == V[VY]) 
    {
        PC += 4;
    } 
    else 
    {
        PC += 2;
    }
}

void Chip8::IN_6XNN() 
{

    u8 NN = (opcode & 0x00FFu);
    u8 VX = (opcode & 0x0F00u) >> 8u;

    V[VX] = NN;
    PC += 2;
}

void Chip8::IN_7XNN() 
{

    u8 NN = (opcode & 0x00FFu);
    u8 VX = (opcode & 0x0F00u) >> 8u;

    V[VX] += NN;
    PC += 2;
}

void Chip8::IN_8XY0() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    V[VX] = V[VY];
    PC += 2;
}

void Chip8::IN_8XY1() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    V[VX] |= V[VY];
    PC += 2;
}

void Chip8::IN_8XY2() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    V[VX] &= V[VY];
    PC += 2;
}

void Chip8::IN_8XY3() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    V[VX] ^= V[VY];
    PC += 2;
}

void Chip8::IN_8XY4() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;
    
    u16 sum = V[VX] + V[VY];

    if (sum > 255U) 
    {
        V[0xF] = 1;
    } 
    else 
    {
        V[0xF] = 0;
    }

    V[VX] = sum & 0xFFu;
    PC += 2;
}

void Chip8::IN_8XY5() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    if (V[VX] > V[VY]) 
    {
        V[0xF] = 1;
    } 
    else 
    {
        V[0xF] = 0;
    }

    V[VX] -= V[VY];
    PC += 2;
}

void Chip8::IN_8XY6() 
{
    u8 VX = (opcode & 0x0F00u) >> 8u;
    V[0xF] = V[VX] & 0x1u;
    V[VX] >>= 1;
    PC += 2;
}

void Chip8::IN_8XY7() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    if (V[VY] > V[VX]) 
    {
        V[0xF] = 1;
    } 
    else 
    {
        V[0xF] = 0;
    }

    V[VX] = V[VY] - V[VX];
    PC += 2;
}

void Chip8::IN_8XYE() 
{
    u8 VX = (opcode & 0x0F00u) >> 8u;
    V[0xF] = (V[VX] & 0x80u) >> 7u;
    V[VX] <<= 1;
    PC += 2;
}

void Chip8::IN_9XY0() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;

    if (V[VX] != V[VY]) 
    {
        PC += 4;
    } 
    else 
    {
        PC += 2;
    }
}

void Chip8::IN_ANNN() 
{

    u16 address = (opcode & 0x0FFF);
    I = address;
    PC += 2;
}

void Chip8::IN_BNNN() 
{

    u16 address = (opcode & 0x0FFF);
    PC = V[0] + address;

}

void Chip8::IN_CXNN() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 NN = (opcode & 0x00FFu);

    u8 random_number = (rand() % (0xFF + 1));

    V[VX] = NN & random_number;
    PC += 2;
    
}

void Chip8::IN_DXYN() 
{
    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 VY = (opcode & 0x00F0u) >> 4u;
    u8 X_pos = V[VX] % 64;
    u8 Y_pos = V[VY] % 32;
    u8 sprite_height = (opcode & 0x000Fu);
    V[0xF] = 0;

    for (int row = 0; row < sprite_height; row++) 
    {
        u8 pixel = memory[I + row];
        for (int column = 0; column < 8; column++) 
        {
            u8 sprite_pixel = pixel & (0x80u >> column);
            if (sprite_pixel != 0) 
            {
                int x = (X_pos + column) % 64;
                int y = (Y_pos + row) % 32;
                int index = y * 64 + x;

                if (display[index] == 1) 
                {
                    V[0xF] = 1;
                }
                display[index] ^= 1;
            }
        }
    }
    PC += 2;
}

void Chip8::IN_EX9E() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;

    if (keypad[V[VX]] != 0) 
    {
        PC += 4;
    } 
    else 
    {
        PC += 2;
    }
}

void Chip8::IN_EXA1() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;

    if (keypad[V[VX]] == 0) 
    {
        PC += 4;
    } 
    else 
    {
        PC += 2;
    }

}

void Chip8::IN_FX07() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    V[VX] = delay_timer;
    PC += 2;

}

void Chip8::IN_FX0A() 
{
    u8 VX = (opcode & 0x0F00u) >> 8u;
    bool key_pressed = false;

    for (int i = 0; i < 16; i++) 
    {
        if (keypad[i]) 
        {
            V[VX] = i;
            key_pressed = true;
            break;
        }
    }

    if (key_pressed) 
    {
        PC += 2;
    }

}

void Chip8::IN_FX15() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    delay_timer = V[VX];

    PC += 2;
}

void Chip8::IN_FX18() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    sound_timer = V[VX];

    PC += 2;
}

void Chip8::IN_FX1E() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;
    I += V[VX];

    PC += 2;

}

void Chip8::IN_FX29() 
{

    u16 VX = (opcode & 0x0F00u) >> 8u;
    I = 0x50 + (V[VX] * 5u);

    PC += 2;
}

void Chip8::IN_FX33() 
{
    u8 VX = (opcode & 0x0F00u) >> 8u;
    u8 value = V[VX];

    memory[I] = value / 100;
    memory[I + 1] = (value / 10) % 10;
    memory[I + 2] = value % 10;

    PC += 2;
}

void Chip8::IN_FX55() 
{

    u8 VX = (opcode & 0x0F00u) >> 8u;

    for (int i = 0; i < VX; i++) 
    {
        memory[I + i] = V[i];
    }

    PC += 2;

}

void Chip8::IN_FX65() {

    u8 VX = (opcode & 0x0F00u) >> 8u;

    for (int i = 0; i < VX; i++) 
    {
        V[i] = memory[I + i];
    }

    PC += 2;

}

void Chip8::function_pointer() {
    table[0x0] = &Chip8::Table0;
    table[0x1] = &Chip8::IN_1NNN;
    table[0x2] = &Chip8::IN_2NNN;
    table[0x3] = &Chip8::IN_3XNN;
    table[0x4] = &Chip8::IN_4XNN;
    table[0x5] = &Chip8::IN_5XY0;
    table[0x6] = &Chip8::IN_6XNN;
    table[0x7] = &Chip8::IN_7XNN;
    table[0x8] = &Chip8::Table8;
    table[0x9] = &Chip8::IN_9XY0;
    table[0xA] = &Chip8::IN_ANNN;
    table[0xB] = &Chip8::IN_BNNN;
    table[0xC] = &Chip8::IN_CXNN;
    table[0xD] = &Chip8::IN_DXYN;
    table[0xE] = &Chip8::TableE;
    table[0xF] = &Chip8::TableF;

    for (size_t i = 0; i < 0xE; i++) 
    {
        table0[i] = &Chip8::IN_NULL;
        table8[i] = &Chip8::IN_NULL;
        tableE[i] = &Chip8::IN_NULL;
    }

    table0[0x0] = &Chip8::IN_00E0;
    table0[0xE] = &Chip8::IN_00EE;

    for (int i = 0; i < 8; ++i) 
    {
        table8[i] = table8_instructions[i];
    }
    table8[0xE] = &Chip8::IN_8XYE;

    tableE[0x1] = &Chip8::IN_EXA1;
    tableE[0xE] = &Chip8::IN_EX9E;

    for (size_t i = 0; i < 0x100; i++) 
    {
        tableF[i] = &Chip8::IN_NULL;
    }

    tableF[0x07] = &Chip8::IN_FX07;
	tableF[0x0A] = &Chip8::IN_FX0A;
	tableF[0x15] = &Chip8::IN_FX15;
	tableF[0x18] = &Chip8::IN_FX18;
	tableF[0x1E] = &Chip8::IN_FX1E;
	tableF[0x29] = &Chip8::IN_FX29;
	tableF[0x33] = &Chip8::IN_FX33;
	tableF[0x55] = &Chip8::IN_FX55;
	tableF[0x65] = &Chip8::IN_FX65;
}

void Chip8::IN_NULL() {}

void Chip8::Table0() 
{
	((*this).*(table0[opcode & 0x000Fu]))();
}

void Chip8::Table8() 
{
    ((*this).*(table8[opcode & 0x000Fu]))();
}

void Chip8::TableE() 
{
    ((*this).*(tableE[opcode & 0x000Fu]))();
}

void Chip8::TableF() 
{
    ((*this).*(tableF[opcode & 0x00FFu]))();
}



