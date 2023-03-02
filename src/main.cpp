#pragma once

#include "hardware.h"
#include <fstream>
#include <assert.h>
#include <stddef.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <chrono>
#include <random>
#include <SDL2/SDL.h>

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
        exit(EXIT_FAILURE);
    }

    int videoScale = stoi(argv[1]);
    int cycleDelay = stoi(argv[2]);
    char *romFilename = argv[3];

    Platform platform("CHIP-8 Emulator", VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

    Chip8 chip8;
    chip8.loadrom(romFilename);

    int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

    auto lastCycleTime = chrono::high_resolution_clock::now();
    bool quit = false;

    while (!quit)
    {
        quit = platform.process_input(chip8.keypad);

        auto currentTime = chrono::high_resolution_clock::now();
        float dt = chrono::duration<float, chrono::milliseconds::period>(currentTime - lastCycleTime).count();

        if (dt > cycleDelay)
        {
            lastCycleTime = currentTime;

            chip8.Cycle();

            platform.update(chip8.video, videoPitch);
        }
    }

    return 0;
}