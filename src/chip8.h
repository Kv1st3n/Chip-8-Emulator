#ifndef CHIP8_H // CHIP_8_H ?
#define CHIP8_H

#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;

class Chip8 
{
    private:
        u16 stack[16];
        u16 stack_pointer;

        u8 memory[4096];
        u8 V[16]; // variable registers

        u16 PC; // program counter
        u16 I; // Index register  
        u16 opcode;

        u8 delay_timer;
        u8 sound_timer;


        void clear_stack(u16 &stack, u8 &V, u8 &keypad);
        void clear_display(u8 &display);
        void clear_memory(u8 &memory);
        void load_font(u8 &memory);
        void function_pointer();

    public:
        Chip8();
        ~Chip8();
        u8 display[64 * 32];
        u8 keypad[16];
        
        void start_chip8();
        void run_cycle();
        bool load_game_rom(const char *file_path);

        // instructions of Chip8
        void IN_0NNN();
        void IN_00E0();
        void IN_00EE();

        void IN_1NNN();
        void IN_2NNN();
        void IN_3XNN();
        void IN_4XNN();
        void IN_5XY0();
        void IN_6XNN();
        void IN_7XNN();

        void IN_8XY0();
        void IN_8XY1();
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

        void IN_NULL();
        void Table0();
        void Table8();
        void TableE();
        void TableF();

        typedef void (Chip8::*Instruction_function)();
        using Instruction_function_table8 = void(Chip8::*)();

        Instruction_function_table8 table8_instructions[8] = {
            &Chip8::IN_8XY0, &Chip8::IN_8XY1, &Chip8::IN_8XY2, &Chip8::IN_8XY3,
            &Chip8::IN_8XY4, &Chip8::IN_8XY5, &Chip8::IN_8XY6, &Chip8::IN_8XY7
        };

        Instruction_function table[0xF + 1];
        Instruction_function table0[0x10];
        Instruction_function table8[0x10];
        Instruction_function tableE[0x10];
        Instruction_function tableF[0x100];
};


#endif