#ifndef KOJO_XFBIN_PAGE_HPP
#define KOJO_XFBIN_PAGE_HPP

#include <xfbin/chunk.hpp>

namespace kojo::nucc {

struct Page {
	std::uint32_t chunk_map_offset{0};
	std::uint32_t extra_map_offset{0};

	std::vector<Chunk> chunks{};

	Page() = default;

	Page(std::uint32_t _chunk_map_offset, std::uint32_t _extra_map_offset)
		: chunk_map_offset(_chunk_map_offset), extra_map_offset(_extra_map_offset) {}

	Chunk& add_chunk(const ChunkType type, std::string_view path, std::string_view name,
		const std::uint16_t version, const std::uint16_t unk, Binary data)
	{
		return chunks.emplace_back(type, std::string(path), std::string(name),
			version, unk, std::move(data));
	}
};

}

#endif
