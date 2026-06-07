#ifndef KOJO_XFBIN_CHUNK_TEXTURE_HPP
#define KOJO_XFBIN_CHUNK_TEXTURE_HPP

#include <xfbin/chunk.hpp>

namespace kojo::nucc {

class ChunkTexture {
public:
	std::uint16_t unk00;
	std::uint16_t width;
	std::uint16_t height;
	std::uint16_t unk06;
	std::uint32_t size;
	std::span<const std::byte> data;

	[[nodiscard]] static auto from(const Chunk& chunk) noexcept
		-> std::expected<ChunkTexture, XfbinError>;

private:
	std::vector<std::byte> data_store;
};

static_assert(IsChunkParseable<ChunkTexture>);

}

#endif
