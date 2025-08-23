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

DecodedInstruction decode(std::uint16_t address, std::uint16_t opcode);
std::vector<DecodedInstruction> disassemble(std::uint16_t start_address, const std::vector<std::uint8_t>& bytes);
std::string format_instruction(const DecodedInstruction& instruction);
bool is_jump_or_call(std::uint16_t opcode);
bool touches_register(std::uint16_t opcode, std::uint8_t register_index);
