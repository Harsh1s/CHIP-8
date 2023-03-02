#ifndef HARDWARE_H
#define HARDWARE_H

#pragma once

#include <cstdint>
#include <random>
#include <SDL2/SDL.h>


using namespace std;

const int START_ADDRESS = 0x200;
const int FONTSET_START_ADDRESS = 0x50;
const int VIDEO_HEIGHT = 32;
const int VIDEO_WIDTH = 64;

class Chip8
{
public:
	Chip8();					  // Constructor
	void loadrom(char *filename); // Let's load the rom
	void Cycle();				  // one CPU cycle

	uint8_t keypad[16]{}; // 1 to F

	uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT]{}; // Video buffer

private:
	uint8_t registers[16]{}; // CPU registerws
	uint8_t memory[4096]{};	 // Memory
	uint16_t index{};		 // Address of register in use
	uint16_t pc{};			 // Program Counter
	uint16_t stack[16]{};	 // Stack for CALL and RET
	uint8_t sp{};			 // Stack Pointer
	uint8_t delayTimer{};	 // decrement when != 0, cycle clock rate
	uint8_t soundTimer{};	 // tone buzzes when != 0
	uint16_t opcode;		 // Operation Code

	void Table0();
	void Table8();
	void TableE();
	void TableF();

	typedef void (Chip8::*Chip8Func)();
	Chip8Func table[0xF + 1];
	Chip8Func table0[0xE + 1];
	Chip8Func table8[0xE + 1];
	Chip8Func tableE[0xE + 1];
	Chip8Func tableF[0x65 + 1];

	// Operation Codes
	void OP_NULL(); // Do nothing
	void OP_00E0(); // CLS
	void OP_00EE(); // RET
	void OP_1nnn(); // JP address
	void OP_2nnn(); // CALL address
	void OP_3xkk(); // SE Vx, byte
	void OP_4xkk(); // SNE Vx, byte
	void OP_5xy0(); // SE Vx, Vy
	void OP_6xkk(); // LD Vx, byte
	void OP_7xkk(); // ADD Vx, byte
	void OP_8xy0(); // LD Vx, Vy
	void OP_8xy1(); // OR Vx, Vy
	void OP_8xy2(); // AND Vx, Vy
	void OP_8xy3(); // XOR Vx, Vy
	void OP_8xy4(); // ADD Vx, Vy
	void OP_8xy5(); // SUB Vx, Vy
	void OP_8xy6(); // SHR Vx
	void OP_8xy7(); // SUBN Vx, Vy
	void OP_8xyE(); // SHL Vx
	void OP_9xy0(); // SNE Vx, Vy
	void OP_Annn(); // LD I, address
	void OP_Bnnn(); // JP V0, address
	void OP_Cxkk(); // RND Vx, byte
	void OP_Dxyn(); // DRW Vx, Vy, height
	void OP_Ex9E(); // SKP Vx
	void OP_ExA1(); // SKNP Vx
	void OP_Fx07(); // LD Vx, DT
	void OP_Fx0A(); // LD Vx, K
	void OP_Fx15(); // LD DT, Vx
	void OP_Fx18(); // LD ST, Vx
	void OP_Fx1E(); // ADD I, Vx
	void OP_Fx29(); // LD F, Vx
	void OP_Fx33(); // LD B, Vx
	void OP_Fx55(); // LD [I], Vx
	void OP_Fx65(); // LD Vx, [I]

	default_random_engine randGen;
	uniform_int_distribution<uint8_t> randByte;
};

class Platform
{
    public:

        Platform(const char* title, unsigned windowWidth, unsigned windowHeight,
                    unsigned textureWidth, unsigned textureHeight);

        ~Platform();

        void update(const void* buffer, int pitch);
        bool process_input(uint8_t* keys);

    private:

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
};




#endif /* HARDWARE_H */