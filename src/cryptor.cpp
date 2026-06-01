#include <xfbin/detail/cryptor.hpp>

using namespace kojo::nucc;
using namespace kojo::type_abbreviations;

void Cryptor::reset_state()
{
	v1 = (key[0] << 24) | (key[2] << 16) | (key[4] << 8) | key[6];
	v1 = (0x1da597 * (v1 | key[6])) ^ ((v1 | key[6]) >> 5);
	v2 = (0x1da597 * v1) ^ ((v1 >> 5) + 0x85c9c2);
	v3 = (0x1da597 * v2) ^ ((v2 >> 5) + 0x10b9384);
	v4 = (0x1da597 * v3) ^ ((v3 >> 5) + 0x1915d46);
}

void Cryptor::crypt(const std::byte* data, const std::size_t size)
{
	auto mut_data = (std::uint8_t*)data;
	for (std::size_t i = 0; i < size;) {
		const std::array<u8, 4> keystream = roll_key();

		const auto n = std::min<std::size_t>(4, size - i);
		for (size_t j = 0; j < n; ++j) {
			mut_data[i + j] = mut_data[i + j] ^ keystream[j];
		}
		i += n;
	}
}

auto Cryptor::roll_key() -> std::array<u8, 4>
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
	std::array<u8, 4> xor_key;
	xor_key[0] = static_cast<u8>(v7 >> 24); // HIBYTE(v7)
	xor_key[1] = static_cast<u8>(v7 >> 16); // BYTE2(v7)
	xor_key[2] = static_cast<u8>(v9 >>  8); // BYTE1(v9)
	xor_key[3] = static_cast<u8>(v9 >>  0); // BYTE0(v9)

	return xor_key;
}
