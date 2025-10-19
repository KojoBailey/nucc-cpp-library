//
// Created by kapil on 15.08.2025.
//

#ifndef CRYPT_HPP
#define CRYPT_HPP
#include <algorithm>
#include <cstdint>

class xfbin_cryptor {
public:
        xfbin_cryptor() = default;

        void initialize(const uint8_t input_key[8])
        {
                std::copy_n(input_key, 8, key);
                reset_state();
        }

        // In-place safe if data_out == data_in.
        void decrypt(uint8_t* data_out, const uint8_t* data_in, size_t size)
        {
                size_t i = 0;
                while (i < size) {
                        uint8_t ks[4];
                        next_bytes(ks);

                        const size_t n = std::min<size_t>(4, size - i);
                        for (size_t k = 0; k < n; ++k) {
                                data_out[i + k] = data_in[i + k] ^ ks[k];
                        }
                        i += n;
                }
        }

        void reset_state()
        {
                v1 = (key[0] << 24) | (key[2] << 16) | (key[4] << 8) | key[6];
                v1 = (0x1da597 * (v1 | key[6])) ^ ((v1 | key[6]) >> 5);
                v2 = (0x1da597 * v1) ^ ((v1 >> 5) + 0x85c9c2);
                v3 = (0x1da597 * v2) ^ ((v2 >> 5) + 0x10b9384);
                v4 = (0x1da597 * v3) ^ ((v3 >> 5) + 0x1915d46);
        }
private:
        uint32_t v1{};
        uint32_t v2{};
        uint32_t v3{};
        uint32_t v4{};
        // uint32_t should_decrypt;

        uint8_t key[8]{};

        void next_bytes(uint8_t ks[4])
        {
                // Save and rotate state
                const uint32_t a = v1;
                const uint32_t d = v4;
                v1 = v2;
                v2 = v3;
                v3 = d;

                // Core update
                const uint32_t t  = a ^ (a << 11);
                const uint32_t v7 = d ^ a ^ (a << 11) ^ (t >> 8);
                const uint32_t v9 = v7 ^ (d >> 19);
                v4 = v9;

                // 4 keystream bytes used for up to 4 data bytes
                ks[0] = static_cast<uint8_t>(v7 >> 24); // HIBYTE(v7)
                ks[1] = static_cast<uint8_t>(v7 >> 16); // BYTE2(v7)
                ks[2] = static_cast<uint8_t>(v9 >>  8); // BYTE1(v9)
                ks[3] = static_cast<uint8_t>(v9 >>  0); // BYTE0(v9)
        }
};

#endif //CRYPT_HPP
