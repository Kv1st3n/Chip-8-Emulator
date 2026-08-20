#include "renderer.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::init() {

    SDL_Window *window;
    bool done = false;

    SDL_Init(SLD_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Chip8 Emulator",
        640,
        320,
        SDL_WINDOW_OPENGL
    );

    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

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