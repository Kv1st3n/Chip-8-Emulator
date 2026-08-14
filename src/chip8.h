#ifndef CHIP_8_H
#define CHIP_8_H

#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;

class Chip8 {
private:
    u16 stack[16];
    u16 stack_pointer;

    u8 memory[4096];
    u8 V[16]; // variable registers

    u8 PC; // program counter
    u8 I; // Index register  
    u8 opcode;

    u8 delay_timmer;
    u8 sound_timer;

    void init();

public:
    u8 display[64 * 32];
    u8 keypad[16];
    
    void run_cycle();
    bool load_game_rom(const char *file_path);
};


#endif