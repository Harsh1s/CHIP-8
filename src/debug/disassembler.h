#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace chip8::debug {

struct DecodedInstruction {
    std::uint16_t address;
    std::uint16_t opcode;
    std::string mnemonic;
    std::string operands;
};

