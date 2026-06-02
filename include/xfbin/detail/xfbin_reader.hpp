#ifndef KOJO_XFBIN_READER_HPP
#define KOJO_XFBIN_READER_HPP

#include <xfbin/xfbin.hpp>
#include <xfbin/detail/cryptor.hpp>

#include <memory>

namespace kojo::nucc {

class XfbinReader {
public:
	XfbinReader(BinaryView _data, std::array<std::uint8_t, Cryptor::KEY_SIZE> _crypt_key)
		: data(_data), crypt_key(_crypt_key) {}

	[[nodiscard]] auto parse() && -> std::expected<Xfbin, XfbinError>;

private:
	static constexpr std::size_t CHUNK_HEADER_SIZE{12};

	BinaryView data;
	Xfbin result;

	bool is_encrypted{false};
	const std::array<std::uint8_t, Cryptor::KEY_SIZE> crypt_key;
	std::unique_ptr<Cryptor> cryptor;

	[[nodiscard]] auto parse_header() -> std::expected<void, XfbinError>;
	[[nodiscard]] auto parse_index()  -> std::expected<void, XfbinError>;
	[[nodiscard]] auto parse_chunks() -> std::expected<void, XfbinError>;
};

}

#endif
