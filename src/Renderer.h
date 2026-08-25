#ifndef RENDERER_H
#define RENDERER_H

#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "chip8.h"

class Renderer
{
    public:

        // constructor
        Renderer(int width, int height, int texture_width, int texture_height);
        // deconstructor
        ~Renderer();

        void init();
        void handle_sound();
        void update_screen();
        void keydown_input_handler(SDL_Event event, Chip8 chip8);
        void keyup_input_handler(SDL_Event event, Chip8 chip8);

    private:
        SDL_Window* window{};
        SDL_Renderer* renderer{};
        SDL_Texture* texture{};
};

#endif