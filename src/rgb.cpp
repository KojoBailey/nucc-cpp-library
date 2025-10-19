#include <nucc/utils/rgb.hpp>

#include <format>
#include <sstream>

using namespace nucc;

std::uint32_t rgb::consolidate() const
{
        return alpha | ((blue | ((green | (red << 8)) << 8)) << 8);
}

rgb rgb::from_hex_code(std::string hex_code)
{
        std::erase(hex_code, '#');
        std::stringstream buffer;
        buffer << std::hex << hex_code.substr(0, 2);
        buffer >> red;
        buffer.clear();
        buffer << std::hex << hex_code.substr(2, 2);
        buffer >> green;
        buffer.clear();
        buffer << std::hex << hex_code.substr(4, 2);
        buffer >> blue;
        return *this;
}

std::string rgb::to_hex_code() const
{
        return std::format("#{:02x}{:02x}{:02x}", red, green, blue);
}