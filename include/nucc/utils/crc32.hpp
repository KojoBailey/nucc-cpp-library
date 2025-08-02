#ifndef KOJO_NUCC_CRC32
#define KOJO_NUCC_CRC32

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

namespace nucc {

inline constexpr std::array<std::uint32_t, 256> generate_crc32_table() {
    std::array<std::uint32_t, 256> buffer{};

    constexpr std::uint32_t POLYNOMIAL = 0x4C11DB7;

    for (int i = 0; i < 256; ++i) {
        std::uint32_t crc = static_cast<std::uint32_t>(i) << 24;
        for (int j = 0; j < 8; ++j) {
            crc = (crc & 0x80000000) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
        }
        buffer[i] = crc;
    }

    return buffer;
}

constexpr std::array<std::uint32_t, 256> crc32_hash_table = generate_crc32_table();

class crc32 {
public:
    crc32() = default;
    crc32(std::uint32_t _id) : m_id(_id) {}
    crc32(std::string str) {
        load(str);
    }

    void load(std::uint32_t _id);
    void load(std::string str);

    std::uint32_t id() const { return m_id; }
    std::string string() const;

    static constexpr std::uint32_t hash(std::string_view str) {
        std::uint32_t crc = 0xFFFFFFFF;

        for (std::size_t i = 0; i < str.length(); ++i) {
            std::uint8_t byte = static_cast<std::uint8_t>(str[i]);
            std::uint8_t index = static_cast<std::uint8_t>((crc >> 24) ^ byte);
            crc = (crc << 8) ^ crc32_hash_table[index];
        }

        return ~crc;
    }

private:
    std::uint32_t m_id{0};
};

}

#endif // KOJO_NUCC_CRC32