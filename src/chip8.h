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
    void clear_stack(u16 stack, u8 V, u8 keypad);
    void clear_display(u8 display);
    void clear_memory(u8 memory);
    void load_font(u8 memory);

public:
    u8 display[64 * 32];
    u8 keypad[16];
    
    void run_cycle();
    bool load_game_rom(const char *file_path);

    // instructions of Chip8
    void IN_0NNN(); //
    void IN_00E0(); //
    void IN_00EE(); //

    void IN_1NNN(); //
    void IN_2NNN(); //
    void IN_3XNN(); //
    void IN_4XNN(); //
    void IN_5XY0(); //
    void IN_6XNN(); //
    void IN_7XNN(); //

    void IN_8XY0(); //
    void IN_8XY1(); // todo, fix remaining instructions
    void IN_8XY2();
    void IN_8XY3();
    void IN_8XY4();
    void IN_8XY5();
    void IN_8XY6();
    void IN_8XY7();
    void IN_8XYE();

    void IN_9XY0();

    void IN_ANNN();
    void IN_BNNN();
    void IN_CXNN();
    void IN_DXYN();

    void IN_EX9E();
    void IN_EXA1();

    void IN_FX07();
    void IN_FX0A();
    void IN_FX15();
    void IN_FX18();
    void IN_FX1E();
    void IN_FX29();
    void IN_FX33();
    void IN_FX55();
    void IN_FX65();

};


#endif