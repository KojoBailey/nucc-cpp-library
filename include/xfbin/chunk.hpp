#ifndef KOJO_XFBIN_CHUNK_HPP
#define KOJO_XFBIN_CHUNK_HPP

#include <xfbin/chunk_type.hpp>

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <string_view>
#include <ranges>

namespace kojo::nucc {

struct Chunk {
	ChunkType type;
	std::string path;
	std::string name;
	std::uint16_t version;
	std::uint16_t unk;

	std::vector<std::byte> data;
};

}

#endif
