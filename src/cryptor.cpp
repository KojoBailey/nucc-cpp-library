#include <Xfbin/xfbin.hpp>

using namespace kojo::binary_types;

namespace kojo {

void Xfbin::cryptor::load(const std::array<u8, 8> key)
{
	m_key = key;
	reset_state();
}

void Xfbin::cryptor::reset_state()
{
	v1 = (m_key[0] << 24) | (m_key[2] << 16) | (m_key[4] << 8) | m_key[6];
	v1 = (0x1da597 * (v1 | m_key[6])) ^ ((v1 | m_key[6]) >> 5);
	v2 = (0x1da597 * v1) ^ ((v1 >> 5) + 0x85c9c2);
	v3 = (0x1da597 * v2) ^ ((v2 >> 5) + 0x10b9384);
	v4 = (0x1da597 * v3) ^ ((v3 >> 5) + 0x1915d46);
}

void Xfbin::cryptor::crypt(u8* data_out, const size_t size)
{
	const u8* data_in = data_out;

	size_t i = 0;
	while (i < size) {
		std::array<u8, 4> ks;
		roll_key(ks);

		const auto n = std::min<size_t>(4, size - i);
		for (size_t k = 0; k < n; ++k) {
			data_out[i + k] = data_in[i + k] ^ ks[k];
		}
		i += n;
	}
}

void Xfbin::cryptor::roll_key(std::array<u8, 4> xor_key)
{
	// Save and rotate state
	const u32 a = v1;
	const u32 d = v4;
	v1 = v2;
	v2 = v3;
	v3 = d;

	// Core update
	const u32 t  = a ^ (a << 11);
	const u32 v7 = d ^ a ^ (a << 11) ^ (t >> 8);
	const u32 v9 = v7 ^ (d >> 19);
	v4 = v9;

	// 4 keystream bytes used for up to 4 data bytes
	xor_key[0] = static_cast<u8>(v7 >> 24); // HIBYTE(v7)
	xor_key[1] = static_cast<u8>(v7 >> 16); // BYTE2(v7)
	xor_key[2] = static_cast<u8>(v9 >>  8); // BYTE1(v9)
	xor_key[3] = static_cast<u8>(v9 >>  0); // BYTE0(v9)
}

}