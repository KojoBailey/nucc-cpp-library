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

	Chunk(const ChunkType _type, std::string_view _path, std::string_view _name,
		const std::uint16_t _version, const std::uint16_t _unk, std::span<std::byte> _data)
		: type{_type}, path{std::string{_path}}, name{std::string{_name}},
			version{_version}, unk{_unk}, data{std::from_range, _data} {}
};

}

#endif
