#include <nucc/crc32.hpp>

#include <kojo/binary.hpp>

#include <bit>
#include <format>
#include <regex>

using namespace nucc;

void crc32::load(std::uint32_t _id) {
    m_id = _id;
}
void crc32::load(std::string str) {
    if (std::regex_match(str, std::regex("^([0-9a-fA-F]{8})$"))) {
        m_id = std::stoul(str, nullptr, 16);
    } else {
        m_id = hash(str);
        kojo::binary::set_endian(id, std::endian::big);
    }
}

std::string crc32::string() {
    return std::format("{:08x}", m_id);
}

constexpr std::uint32_t crc32::hash(std::string_view str) {
    std::uint32_t crc = 0xFFFFFFFF;

    for (std::size_t i = 0; i < str.length(); ++i) {
        std::uint8_t byte = static_cast<std::uint8_t>(str[i]);
        std::uint8_t index = static_cast<std::uint8_t>((crc >> 24) ^ byte);
        crc = (crc << 8) ^ crc32_hash_table[index];
    }

    return ~crc;
}