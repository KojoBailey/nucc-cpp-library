#ifndef KOJO_XFBIN_CHUNK_HPP
#define KOJO_XFBIN_CHUNK_HPP

#include <xfbin/chunk_type.hpp>

#include <kojo/binary.hpp>

namespace kojo::nucc {

struct Chunk {
	ChunkType type;
	std::string path;
	std::string name;
	std::uint16_t version;
	std::uint16_t unk;
	Binary data;

	Chunk(const ChunkType _type, std::string_view _path, std::string_view _name,
		const std::uint16_t _version, const std::uint16_t _unk, Binary _data)
		: type(_type), path(std::string(_path)), name(std::string(_name)),
			version(_version), unk(_unk), data(std::move(_data)) {}
};

}

#endif
