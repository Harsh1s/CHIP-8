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

