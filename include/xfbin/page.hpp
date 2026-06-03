#ifndef KOJO_XFBIN_PAGE_HPP
#define KOJO_XFBIN_PAGE_HPP

#include <xfbin/chunk.hpp>

namespace kojo::nucc {

struct Page {
	std::uint32_t chunk_map_offset{0};
	std::uint32_t extra_map_offset{0};

	std::vector<Chunk> chunks;

	Page() = default;

	Page(std::uint32_t _chunk_map_offset, std::uint32_t _extra_map_offset)
		: chunk_map_offset{_chunk_map_offset}, extra_map_offset{_extra_map_offset} {}

	template<typename... Args>
	auto add_chunk(Args&&... args)
		-> Chunk&
	{
		return chunks.emplace_back(std::forward<Args>(args)...);
	}
};

}

#endif
