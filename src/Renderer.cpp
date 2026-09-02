#include "renderer.h"
#include "chip8.h"
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

typedef uint8_t u8;

SDL_Keycode keymap[16] = 
{
    SDLK_X,   // 0x0
    SDLK_1,   // 0x1
    SDLK_2,   // 0x2
    SDLK_3,   // 0x3
    SDLK_Q,   // 0x4
    SDLK_W,   // 0x5
    SDLK_E,   // 0x6
    SDLK_A,   // 0x7
    SDLK_S,   // 0x8
    SDLK_D,   // 0x9
    SDLK_Z,   // 0xA
    SDLK_C,   // 0xB
    SDLK_4,   // 0xC
    SDLK_R,   // 0xD
    SDLK_F,   // 0xE
    SDLK_V    // 0xF
};

Renderer::Renderer(int width, int height, int texture_width, int texture_height) 
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Chip8 Emulator", width, height, 0);

    if (window == NULL) 
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, NULL);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texture_width, texture_height);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);

}

Renderer::~Renderer() 
{   
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::update_screen(Chip8 &chip8, uint32_t* pixels)
{   

    for (int i = 0; i < 2048; ++i) 
    {
        u8 pixel = chip8.display[i];
        pixels[i] = pixel ? 0xFFFFFFFF : 0x000000FF;
    }

    uint64_t pitch = 64 * sizeof(uint32_t);

    SDL_UpdateTexture(texture, NULL, pixels, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void Renderer::init(char const* rom_name) 
{
    bool done = false;
    uint32_t pixels[2048];

    Chip8 chip8;
    chip8.start_chip8();
    chip8.load_game_rom(rom_name);

    while (!done) 
    {
        SDL_Event event;

        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_EVENT_QUIT) 
            {
                done = true;
            }

            keydown_input_handler(event, chip8);
            keyup_input_handler(event, chip8);
        }

        chip8.run_cycle();
        update_screen(chip8, pixels);
    }
}

// checks if any key is being pressed
void Renderer::keydown_input_handler(SDL_Event &event, Chip8 &chip8) 
{
    if (event.type == SDL_EventType::SDL_EVENT_KEY_DOWN) 
    {
        if (event.key.key == SDLK_ESCAPE) 
        {
            exit(0);
        }

        for (int i = 0; i < 16; ++i) 
        {
            if (event.key.key == keymap[i]) 
            {
                chip8.keypad[i] = 1;
            }
        }
    }
}

void Renderer::keyup_input_handler(SDL_Event &event, Chip8 &chip8) 
{
    if (event.type == SDL_EventType::SDL_EVENT_KEY_UP) 
    {
        for (int i = 0; i < 16; ++i) 
        {
            if (event.key.key == keymap[i]) 
            {
                chip8.keypad[i] = 0;
            }
        }
    }
}