#ifndef KOJO_NUCC_HASH
#define KOJO_NUCC_HASH

#include <kojo/binary.hpp>

#include <bit>
#include <cstdint>
#include <string_view>
#include <vector>

namespace nucc {

static std::vector<std::uint32_t> hash_array;

inline std::uint32_t hash(std::string_view str) {
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

    if (kojo::system_endian() == std::endian::little) {
        return kojo::byteswap(v2);
    }
    return v2;
}

} // namespace nucc

#endif // KOJO_NUCC_HASH