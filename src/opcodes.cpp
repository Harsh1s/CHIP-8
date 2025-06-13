#pragma once

#include "hardware.h"
#include <fstream>
#include <assert.h>
#include <stddef.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <cstring>
#include <SDL2/SDL.h>

void Chip8::Table0()
{
    ((*this).*(table0[opcode & 0x000Fu]))();
}

void Chip8::Table8()
{
    ((*this).*(table8[opcode & 0x000Fu]))();
}

void Chip8::TableE()
{
    ((*this).*(tableE[opcode & 0x000Fu]))();
}

void Chip8::TableF()
{
    ((*this).*(tableF[opcode & 0x00FFu]))();
}

void Chip8::OP_NULL() // Don't do anything
{
}

void Chip8::OP_00E0()
{
    memset(video, 0, sizeof(video)); // memset the F out of video buffer
}

void Chip8::OP_00EE()
{
    --sp;
    pc = stack[sp]; // RET makes stack pointer go brr brr
}

void Chip8::OP_1nnn()
{
    uint16_t address = opcode & 0x0FFFu; // to extract address from OPCode, make 1st 4 bits 0

    pc = address; // jump has no interaction with the stack
}

void Chip8::OP_2nnn()
{
    uint16_t address = opcode & 0x0FFFu;

    stack[sp] = pc; // during cycle() PC already has instruction after call, otherwise infinite loop would be formed
    ++sp;
    pc = address;
}

void Chip8::OP_3xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t ifkk = opcode & 0x00FFu;       // extract kk using bitwise AND operator

    if (registers[Vx] == ifkk)
    {
        pc += 2;
    }
}

void Chip8::OP_4xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t ifkk = opcode & 0x00FFu;       // extract kk using bitwise AND operator

    if (registers[Vx] != ifkk)
    {
        pc += 2;
    }
}

void Chip8::OP_5xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; // extract y using bitwise AND operator and RIght Shift operator

    if (registers[Vx] == registers[Vy])
    {
        pc += 2;
    }
}

void Chip8::OP_6xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t kk = opcode & 0x00FFu;         // extract kk using bitwise AND operator

    registers[Vx] = kk;
}

void Chip8::OP_7xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t kk = opcode & 0x00FFu;         // extract kk using bitwise AND operator

    registers[Vx] += kk;
}

void Chip8::OP_8xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; // extract y using bitwise AND operator and RIght Shift operator

    registers[Vx] = registers[Vy];
}

void Chip8::OP_8xy1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; // extract y using bitwise AND operator and RIght Shift operator

    registers[Vx] |= registers[Vy]; // Bitwise OR
}

void Chip8::OP_8xy2()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; // extract y using bitwise AND operator and RIght Shift operator

    registers[Vx] &= registers[Vy]; // Bitwise AND
}

void Chip8::OP_8xy3()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; // extract y using bitwise AND operator and RIght Shift operator

    registers[Vx] ^= registers[Vy]; // Bitwise XOR
}

void Chip8::OP_8xy4()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; // extract y using bitwise AND operator and RIght Shift operator

    uint16_t sum = registers[Vx] + registers[Vy];

    if (sum > 255U)
    {
        registers[0xF] = 1; // overflow flag set to 1
    }
    else
    {
        registers[0xF] = 0; // overflow flag set to 0
    }

    registers[Vx] = sum & 0xFFu; // store just the lowest 8 bits of sum into Vx
}

void Chip8::OP_8xy5()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; // extract y using bitwise AND operator and RIght Shift operator

    if (registers[Vx] > registers[Vy])
    {
        registers[0xF] = 1; // NOT borrow flag set to 1
    }
    else
    {
        registers[0xF] = 0; // NOT borrow flag set to 0
    }

    registers[Vx] -= registers[Vy]; // borrow flag needed cuz 0 - 4 = 252 for unsigned int
}

void Chip8::OP_8xy6()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator

    registers[0xF] = (registers[Vx] & 0x1u); // Save Least Significant Bit in VF

    registers[Vx] >>= 1; // division by 2
}

void Chip8::OP_8xy7()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; // extract y using bitwise AND operator and RIght Shift operator

    if (registers[Vy] > registers[Vx])
    {
        registers[0xF] = 1; // NOT borrow flag set to 1
    }
    else
    {
        registers[0xF] = 0; // NOT borrow flag set to 0
    }

    registers[Vx] = registers[Vy] - registers[Vx]; // borrow flag needed cuz 0 - 4 = 252 for unsigned int
}

void Chip8::OP_8xyE()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator

    registers[0xF] = (registers[Vx] & 0x80u) >> 7u; // Save Most Significant Bit in VF

    registers[Vx] <<= 1; // multiplication by 2
}

void Chip8::OP_9xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; // extract y using bitwise AND operator and RIght Shift operator

    if (registers[Vx] != registers[Vy])
    {
        pc += 2;
    }
}

void Chip8::OP_Annn()
{
    uint16_t address = opcode & 0x0FFFu; // extract address using bitwise AND operator and RIght Shift operator

    index = address; // current register
}

void Chip8::OP_Bnnn()
{
    uint16_t address = opcode & 0x0FFFu; // extract address using bitwise AND operator and RIght Shift operator

    pc = registers[0] + address; // JUMP to nnn(address) + V0
}

void Chip8::OP_Cxkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t kk = opcode & 0x00FFu;         // extract kk using bitwise AND operator

    registers[Vx] = randByte(randGen) & kk; // Maybe your own RNG some day??
}

void Chip8::OP_Dxyn() // Dxyn *wink wink*
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; // extract y using bitwise AND operator and RIght Shift operator
    uint8_t n = opcode & 0x000Fu;

    uint8_t xPos = registers[Vx] % VIDEO_WIDTH;  // To wrap, use modulo
    uint8_t yPos = registers[Vy] % VIDEO_HEIGHT; // To wrap, use modulo

    registers[0xF] = 0;

    for (int row = 0; row < n; ++row)
    {
        uint8_t spriteByte = memory[index + row]; // A byte of sprite pixel has 8 columns and we iterate over rows to get individual sprite pixels

        /*

             Let's take the sprite of character F as an example:

             11110000
             10000000
           > 11110000
             10000000
             10000000
                ^

            Here n = 5, number of column is fixed, the row iterator is at 3rd row and the column iterator is at 4th column.
            Hence, we get sprite pixel = 1 (on).

        */

        for (int col = 0; col < 8; ++col)
        {
            uint8_t spritePixel = spriteByte & (0x80u >> col);                         // get the sprite pixel from sprite byte, AND Operator and right shift
            uint32_t *screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)]; // get the screen pixel state from video buffer

            if (spritePixel) // if sprite pixel is on
            {
                if (*screenPixel == 0xFFFFFFFF) // if screen pixel also on (collision)
                {
                    registers[0xF] = 1;
                }

                *screenPixel ^= 0xFFFFFFFF; // XOR with the sprite pixel(cannot directly XOR with 0 or 1 of screen pixel)
            }
        }
    }
}

void Chip8::OP_Ex9E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator

    uint8_t keypress = registers[Vx]; // get the keypress

    if (keypad[keypress])
    {
        pc += 2; // skippity skip
    }
}

void Chip8::OP_ExA1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator

    uint8_t keypress = registers[Vx]; // get the keypress

    if (!keypad[keypress])
    {
        pc += 2; // skippity skip
    }
}

void Chip8::OP_Fx07()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator

    registers[Vx] = delayTimer; // delay timer value is stored in Vx
}

void Chip8::OP_Fx0A()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator

    if (keypad[0])
    {
        registers[Vx] = 0;
    }
    else if (keypad[1])
    {
        registers[Vx] = 1;
    }
    else if (keypad[2])
    {
        registers[Vx] = 2;
    }
    else if (keypad[3])
    {
        registers[Vx] = 3;
    }
    else if (keypad[4])
    {
        registers[Vx] = 4;
    }
    else if (keypad[5])
    {
        registers[Vx] = 5;
    }
    else if (keypad[6])
    {
        registers[Vx] = 6;
    }
    else if (keypad[7])
    {
        registers[Vx] = 7;
    }
    else if (keypad[8])
    {
        registers[Vx] = 8;
    }
    else if (keypad[9])
    {
        registers[Vx] = 9;
    }
    else if (keypad[10])
    {
        registers[Vx] = 10;
    }
    else if (keypad[11])
    {
        registers[Vx] = 11;
    }
    else if (keypad[12])
    {
        registers[Vx] = 12;
    }
    else if (keypad[13])
    {
        registers[Vx] = 13;
    }
    else if (keypad[14])
    {
        registers[Vx] = 14;
    }
    else if (keypad[15])
    {
        registers[Vx] = 15;
    }
    else
    {
        pc -= 2; // if keypress is not detected, PC goes one step back. Then it starts to wait for a keypress again. This creates a loop until a key is actually pressed. Hence waiting for a keypress is implemented.
    }
}

void Chip8::OP_Fx15()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator

    delayTimer = registers[Vx]; // delay timer value set to value in Vx
}

void Chip8::OP_Fx18()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator

    soundTimer = registers[Vx]; // sound timer value set to value in Vx
}

void Chip8::OP_Fx1E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator

    index += registers[Vx]; // Set I = I + Vx
}

void Chip8::OP_Fx29()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t hexdigit = registers[Vx]; // getting the character in Vx

    index = FONTSET_START_ADDRESS + (5 * hexdigit); // Set I = location of sprite for hexadecimal digit in Vx
}

void Chip8::OP_Fx33()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator
    uint8_t number = registers[Vx]; // getting the number in Vx

    memory[index + 2] = number % 10; // Ones-place
    number /= 10;

    memory[index + 1] = number % 10; // Tens-place
    number /= 10;

    memory[index] = number % 10; // Hundreds-place
}

void Chip8::OP_Fx55()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator

    for (uint8_t i = 0; i <= Vx; ++i)
    {
        memory[index + i] = registers[i]; // Ctrl+C, Ctrl+V
    }
}

void Chip8::OP_Fx65()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; // extract x using bitwise AND operator and RIght Shift operator

    for (uint8_t i = 0; i <= Vx; ++i)
    {
        registers[i] = memory[index + i]; // read from memory
    }
}