#ifndef KOJO_XFBIN_CHUNK_MODEL_HPP
#define KOJO_XFBIN_CHUNK_MODEL_HPP

#include <xfbin/chunk.hpp>

namespace kojo::nucc {

class ChunkModel {
public:
	std::uint16_t unk00;
	std::uint16_t rigging_flag;
	std::uint16_t attributes;
	std::uint8_t render_layer;
	std::uint8_t light_mode_id;
	std::uint32_t light_category_flag;
	std::uint32_t clump_index;
	std::uint32_t hit_index;
	std::uint32_t mesh_bone_index;
	std::uint32_t size;
	float bounding_box[6];
	std::span<const std::byte> data;
	std::uint32_t material_count;
	std::vector<std::uint32_t> material_indices;

	[[nodiscard]] static auto from(const Chunk& chunk) noexcept
		-> std::expected<ChunkModel, XfbinError>;

private:
	std::vector<std::byte> data_store;
};

static_assert(IsChunkParseable<ChunkModel>);

}

#endif

