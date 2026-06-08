#ifndef KOJO_XFBIN_CHUNK_HPP
#define KOJO_XFBIN_CHUNK_HPP

#include <xfbin/chunk_type.hpp>
#include <xfbin/detail/error.hpp>

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <string>
#include <vector>

namespace kojo::nucc {

struct Chunk {
	ChunkType type;
	std::string path;
	std::string name;
	std::uint16_t version;
	std::uint16_t unk;

	std::vector<std::byte> data;
};

template<typename T>
concept IsChunkParseable = requires(const Chunk& c) {
	{ T::from(c) } -> std::same_as<std::expected<T, XfbinError>>;
};

}

#endif
