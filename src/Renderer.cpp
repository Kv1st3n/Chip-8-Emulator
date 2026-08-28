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

void Renderer::update_screen(Chip8 chip8, uint32_t* pixels)
{   

    for (int i = 0; i < 2048; ++i) {
        u8 pixel = chip8.display[i];
        pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
    }

    uint64_t pitch = 64 * sizeof(uint32_t);

    SDL_UpdateTexture(texture, NULL, pixels, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void Renderer::init() {

    bool done = false;
    int width = 640, height = 320;
    int texture_width = 64, texture_height = 32;
    uint32_t pixels[2048];
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
        update_screen(chip8, pixels);
    }

}

// checks if any key is being pressed
void Renderer::keydown_input_handler(SDL_Event event, Chip8 chip8) {
    if (event.type == SDL_EventType::SDL_EVENT_KEY_DOWN) {

        if (event.type == SDLK_ESCAPE) {
            exit(0);
        }


        for (int i = 0; i < 16; ++i) {
            if (event.type == keymap[i]) {
                chip8.keypad[i] = 1;
            }
        }
    }
}

void Renderer::keyup_input_handler(SDL_Event event, Chip8 chip8) {
    if (event.type == SDL_EventType::SDL_EVENT_KEY_UP) {
        for (int i = 0; i < 16; ++i) {
            if (event.type == keymap[i]) {
                chip8.keypad[i] = 0;
            }
        }
    }
}