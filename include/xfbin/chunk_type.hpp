#ifndef KOJO_XFBIN_CHUNKTYPE_HPP
#define KOJO_XFBIN_CHUNKTYPE_HPP

#include <optional>
#include <string_view>

namespace kojo::nucc {

enum class ChunkType {
	Null,
	Page,
	Index,
	Binary,
	Texture,
};

constexpr std::string_view chunk_type_to_string(ChunkType type)
{
	switch (type) {
	case ChunkType::Null:
		return "nuccChunkNull";
	case ChunkType::Page:
		return "nuccChunkPage";
	case ChunkType::Index:
		return "nuccChunkIndex";
	case ChunkType::Binary:
		return "nuccChunkBinary";
	case ChunkType::Texture:
		return "nuccChunkTexture";
	}
	return "";
}

constexpr std::optional<ChunkType> chunk_type_from_string(std::string_view str)
{
	if (str == "nuccChunkNull")
		return ChunkType::Null;
	else if (str == "nuccChunkPage") 
		return ChunkType::Page;
	else if (str == "nuccChunkIndex") 
		return ChunkType::Index;
	else if (str == "nuccChunkBinary") 
		return ChunkType::Binary;
	else if (str == "nuccChunkTexture") 
		return ChunkType::Texture;

	return {};
}

};

#endif
