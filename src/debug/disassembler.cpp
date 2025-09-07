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
    switch (family) {
    case 0x0000:
        if (opcode == 0x00E0) return {address, opcode, "CLS", ""};
        if (opcode == 0x00EE) return {address, opcode, "RET", ""};
        break;
    case 0x1000: return {address, opcode, "JP", hex(nnn, 3)};
    case 0x2000: return {address, opcode, "CALL", hex(nnn, 3)};
    case 0x3000: return {address, opcode, "SE", "V" + std::to_string(x) + ", " + hex(kk, 2)};
    case 0x4000: return {address, opcode, "SNE", "V" + std::to_string(x) + ", " + hex(kk, 2)};
    case 0x6000: return {address, opcode, "LD", "V" + std::to_string(x) + ", " + hex(kk, 2)};
    case 0x7000: return {address, opcode, "ADD", "V" + std::to_string(x) + ", " + hex(kk, 2)};
    case 0x8000:
        if (n == 0x0) return {address, opcode, "LD", "V" + std::to_string(x) + ", V" + std::to_string(y)};
        if (n == 0x4) return {address, opcode, "ADD", "V" + std::to_string(x) + ", V" + std::to_string(y)};
        if (n == 0x5) return {address, opcode, "SUB", "V" + std::to_string(x) + ", V" + std::to_string(y)};
        break;
    case 0xA000: return {address, opcode, "LD", "I, " + hex(nnn, 3)};
    case 0xD000: return {address, opcode, "DRW", "V" + std::to_string(x) + ", V" + std::to_string(y) + ", " + std::to_string(n)};
    }
    return decoded;
}
