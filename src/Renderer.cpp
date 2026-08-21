#include "renderer.h"
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

typedef uint8_t u8;

u8 keymap[16] { 
    0
};

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::init() {

    SDL_Window *window;
    bool done = false;

    int width = 640, height = 320;

    SDL_Init(SDL_INIT_VIDEO);

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

    while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }
    }

    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
}