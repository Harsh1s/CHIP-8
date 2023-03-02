#pragma once

#include "hardware.h"
#include <fstream>
#include <assert.h>
#include <stddef.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <SDL2/SDL.h>


void Chip8::Cycle()
{
    opcode = (memory[pc] << 8u) | memory[pc + 1]; // since opcodes are stored using two memory blocks, we'll combine two bytes of memory to get one opcode. Left shift first memory block by 8 bits and take a bitwise OR with second one to combine them.

    pc += 2; // increment the PC before we execute anything

    ((*this).*(table[(opcode & 0xF000u) >> 12u]))(); // pointer table

    if (delayTimer > 0)
    {
        --delayTimer; // Decrement the delay timer
    }

    if (soundTimer > 0)
    {
        --soundTimer; // Decrement the sound timer
    }
}

Platform::Platform(const char* title, unsigned windowWidth, unsigned windowHeight,
                   unsigned textureWidth, unsigned textureHeight)
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title, 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}

Platform::~Platform()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Platform::update(void const* buffer, int pitch)
{
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Platform::process_input(uint8_t* keys)
{
    bool quit = false;

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                quit = true;
            } break;

            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        quit = true;
                    } break;

                    case SDLK_x:
                    {
                        keys[0] = 1;
                    } break;

                    case SDLK_1:
                    {
                        keys[1] = 1;
                    } break;

                    case SDLK_2:
                    {
                        keys[2] = 1;
                    } break;

                    case SDLK_3:
                    {
                        keys[3] = 1;
                    } break;

                    case SDLK_q:
                    {
                        keys[4] = 1;
                    } break;

                    case SDLK_w:
                    {
                        keys[5] = 1;
                    } break;

                    case SDLK_e:
                    {
                        keys[6] = 1;
                    } break;

                    case SDLK_a:
                    {
                        keys[7] = 1;
                    } break;

                    case SDLK_s:
                    {
                        keys[8] = 1;
                    } break;

                    case SDLK_d:
                    {
                        keys[9] = 1;
                    } break;

                    case SDLK_z:
                    {
                        keys[0xA] = 1;
                    } break;

                    case SDLK_c:
                    {
                        keys[0xB] = 1;
                    } break;

                    case SDLK_4:
                    {
                        keys[0xC] = 1;
                    } break;

                    case SDLK_r:
                    {
                        keys[0xD] = 1;
                    } break;

                    case SDLK_f:
                    {
                        keys[0xE] = 1;
                    } break;

                    case SDLK_v:
                    {
                        keys[0xF] = 1;
                    } break;
                }
            } break;

            case SDL_KEYUP:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_x:
                    {
                        keys[0] = 0;
                    } break;

                    case SDLK_1:
                    {
                        keys[1] = 0;
                    } break;

                    case SDLK_2:
                    {
                        keys[2] = 0;
                    } break;

                    case SDLK_3:
                    {
                        keys[3] = 0;
                    } break;

                    case SDLK_q:
                    {
                        keys[4] = 0;
                    } break;

                    case SDLK_w:
                    {
                        keys[5] = 0;
                    } break;

                    case SDLK_e:
                    {
                        keys[6] = 0;
                    } break;

                    case SDLK_a:
                    {
                        keys[7] = 0;
                    } break;

                    case SDLK_s:
                    {
                        keys[8] = 0;
                    } break;

                    case SDLK_d:
                    {
                        keys[9] = 0;
                    } break;

                    case SDLK_z:
                    {
                        keys[0xA] = 0;
                    } break;

                    case SDLK_c:
                    {
                        keys[0xB] = 0;
                    } break;

                    case SDLK_4:
                    {
                        keys[0xC] = 0;
                    } break;

                    case SDLK_r:
                    {
                        keys[0xD] = 0;
                    } break;

                    case SDLK_f:
                    {
                        keys[0xE] = 0;
                    } break;

                    case SDLK_v:
                    {
                        keys[0xF] = 0;
                    } break;
                }
            } break;
        }
    }

    return quit;
}
