#include <nucc/utils/crc32.hpp>

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
        m_id = kojo::binary::set_endian(m_id, std::endian::big);
    }
}

std::string crc32::string() const {
    return std::format("{:08x}", m_id);
}