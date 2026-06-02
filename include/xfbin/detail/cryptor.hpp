#ifndef KOJO_XFBIN_CRYPTOR_HPP
#define KOJO_XFBIN_CRYPTOR_HPP

#include <kojo/binary.hpp>

namespace kojo::nucc {

class Cryptor {
public:
	static constexpr std::size_t KEY_SIZE = 8;

	Cryptor(std::array<std::uint8_t, KEY_SIZE> _key) : key{_key}
	{
		reset_state();
	}

	// En/Decryption use the same algorithm, hence it's just "cryption".
	void crypt(const std::byte* data, std::size_t size);

private:
	const std::array<std::uint8_t, KEY_SIZE> key;
	std::uint32_t v1, v2, v3, v4;

	void reset_state();

	auto roll_key() -> std::array<std::uint8_t, 4>;
};

}

#endif
