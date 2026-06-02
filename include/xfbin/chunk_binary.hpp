#ifndef KOJO_XFBIN_CHUNK_BINARY_HPP
#define KOJO_XFBIN_CHUNK_BINARY_HPP

#include <xfbin/chunk.hpp>
#include <xfbin/detail/error.hpp>

namespace kojo::nucc {

class ChunkBinary {
public:
	std::uint32_t size;
	std::span<const std::byte> data;

	ChunkBinary() = default;

	[[nodiscard]] static auto from(const Chunk& chunk) noexcept
		-> std::expected<ChunkBinary, XfbinError>;

private:
	std::vector<std::byte> data_store;
};

}

#endif
