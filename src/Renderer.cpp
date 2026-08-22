#include "renderer.h"
#include "chip8.h"
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

typedef uint8_t u8;

u8 keymap[16] { 
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_4,
    SDLK_Q,
    SDLK_W,
    SDLK_E,
    SDLK_R,
    SDLK_A,
    SDLK_S,
    SDLK_D,
    SDLK_F,
    SDLK_Z,
    SDLK_X,
    SDLK_C,
    SDLK_V
};

Renderer::Renderer(int width, int height, int texture_width, int texture_height) 
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;

    window = SDL_CreateWindow(
        "Chip8 Emulator",
        width,
        height,
        SDL_WINDOW_OPENGL
    );

    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    }

    // renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // texturer
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 32);
}

Renderer::~Renderer() 
{   
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::init() {

    bool done = false;
    int width = 640, height = 320;
    int texture_width = 64, texture_height = 32;

    char const* rom_name;

    Renderer renderer(width, height, texture_width, texture_height);

    Chip8 chip8;
    chip8.load_game_rom(rom_name);

    while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }
    }

}