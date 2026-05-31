#ifndef KOJO_XFBIN_CHUNKTYPE_HPP
#define KOJO_XFBIN_CHUNKTYPE_HPP

#include <cstdint>
#include <optional>
#include <string_view>

namespace kojo::nucc {

enum class ChunkType : std::uint8_t {
	Null,
	Page,
	Index,
	Binary,
	Texture,
};

constexpr auto chunk_type_to_string(ChunkType type) -> std::string_view
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

constexpr auto chunk_type_from_string(std::string_view str)
	-> std::optional<ChunkType>
{
	if (str == "nuccChunkNull")
		return ChunkType::Null;
	if (str == "nuccChunkPage") 
		return ChunkType::Page;
	if (str == "nuccChunkIndex") 
		return ChunkType::Index;
	if (str == "nuccChunkBinary") 
		return ChunkType::Binary;
	if (str == "nuccChunkTexture") 
		return ChunkType::Texture;

	return {};
}

};

#endif
