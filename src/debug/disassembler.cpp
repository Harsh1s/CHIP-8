#include "disassembler.h"

#include <iomanip>
#include <sstream>

namespace chip8::debug {

namespace {
std::string hex(std::uint16_t value, int width) {
    std::ostringstream out;
    out << "0x" << std::uppercase << std::hex << std::setw(width) << std::setfill('0') << value;
    return out.str();
}
}

DecodedInstruction decode(std::uint16_t address, std::uint16_t opcode) {
    DecodedInstruction decoded{address, opcode, "SYS", hex(opcode & 0x0FFF, 3)};
    const auto family = opcode & 0xF000;
    const auto x = static_cast<std::uint8_t>((opcode >> 8) & 0xF);
    const auto y = static_cast<std::uint8_t>((opcode >> 4) & 0xF);
    const auto n = static_cast<std::uint8_t>(opcode & 0xF);
    const auto kk = static_cast<std::uint8_t>(opcode & 0xFF);
    const auto nnn = static_cast<std::uint16_t>(opcode & 0x0FFF);
