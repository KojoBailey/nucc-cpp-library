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

std::uint32_t crc32::hash(std::string_view str) {
    if (hash_array.empty()) {
        std::uint32_t polynomial = 0x4C11DB7;
        for (int i = 0; i < 256; i++) {
            std::uint32_t k = i << 24;
            for (int j = 0; j < 8; j++) {
                if (k & 0x80000000) {
                    k = (k << 1) ^ polynomial;
                } else {
                    k = (k << 1);
                }
            }
            hash_array.push_back(k);
        }
    }

    std::uint32_t v2; // r8d
    std::int64_t i; // r9
    std::int32_t v4; // eax

    v2 = -1;
    if ( str.size() > 0 ) {
        for ( i = 0; i < str.size(); i++ ) {
            v4 = *(reinterpret_cast<std::uint8_t*>(&v2)+(sizeof v2/sizeof(std::uint8_t) - 1)) ^ (std::uint8_t)str[i];
            if ( v4 > 255 ) {
                v4 = 255;
            }
            v2 = (v2 << 8) ^ hash_array[v4];
        }
    }
    v2 = ~v2;

    if (std::endian::native == std::endian::little) {
        return std::byteswap(v2);
    }
    return v2;
}