#ifndef KOJO_NUCC_CRC32
#define KOJO_NUCC_CRC32

#include <cstdint>

namespace nucc {

class crc32 {
public:
    crc32() = default;
    crc32(std::uint32_t _id) : m_id(_id) {}
    crc32(std::string str) {
        load(str);
    }

    void load(std::uint32_t _id);
    void load(std::string str);

    std::uint32_t id() { return m_id; }
    std::string string();

private:
    static std::vector<std::uint32_t> hash_array;

    std::uint32_t m_id;

    std::uint32_t hash(std::string_view str);
};

}

#endif // KOJO_NUCC_CRC32