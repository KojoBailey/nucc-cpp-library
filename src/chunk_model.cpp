#include <xfbin/chunk_model.hpp>

using namespace kojo;
using namespace kojo::nucc;
using namespace kojo::type_abbreviations;

/* This macro restricts compilation to GCC and Clang.
 * Otherwise usage would be `TRY(file_signature, data.read<str>(4));`
 */
#ifdef __clang__
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wgnu-statement-expression-from-macro-expansion"
#elifdef __GNUC__
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wgnu-statement-expression"
#endif

#define TRY(expr) ({ \
	auto&& _tmp = (expr); \
	if (!_tmp) return std::unexpected{XfbinError::from(_tmp.error())}; \
	*std::move(_tmp); \
})

auto ChunkModel::from(const Chunk& chunk) noexcept
	-> std::expected<ChunkModel, XfbinError>
{
	if (chunk.type != ChunkType::Model) {
		return std::unexpected{
			XfbinError::MismatchedChunkType{ChunkType::Model, chunk.type}
		};
	}

	ChunkModel result;

	BinaryView reader{chunk.data};
	if (chunk.version > 115 && chunk.version < 118) {
		/* [TODO] Fill out */
	} else {
		result.unk00         = TRY(reader.read<u16>(std::endian::big));
		result.rigging_flag  = TRY(reader.read<u16>(std::endian::big));
		result.attributes    = TRY(reader.read<u16>(std::endian::big));
		result.render_layer  = TRY(reader.read<u8>(std::endian::big));
		result.light_mode_id = TRY(reader.read<u8>(std::endian::big));
		if (chunk.version > 115) {
			result.light_category_flag = TRY(reader.read<u32>(std::endian::big));
		} else {
			result.light_category_flag = 0;
		}
		result.clump_index = TRY(reader.read<u32>(std::endian::big));
		result.hit_index = TRY(reader.read<u32>(std::endian::big));
		result.mesh_bone_index = TRY(reader.read<u32>(std::endian::big));
		result.size = TRY(reader.read<u32>(std::endian::big));
		if (result.attributes & 0x04) {
			for (std::size_t i = 0; i < 6; i++) {
				/* [TODO] Add native support for reading floats. */
				const auto f32_as_u32 = TRY(reader.read<u32>(std::endian::big));
				result.bounding_box[i] = *reinterpret_cast<f32*>(&f32_as_u32);
			}
		}
		result.data = reader.get_pos_span(result.size);
		result.material_count = TRY(reader.read<u16>(std::endian::big));
		result.material_indices.reserve(result.material_count);
		for (std::size_t i = 0; i < result.material_count; i++) {
			auto material_index = TRY(reader.read<u32>(std::endian::big));
			result.material_indices.push_back(material_index);
		}
	}

	return result;
}

#ifdef __clang__
#	pragma clang diagnostic pop
#elifdef __GNUC__
#	pragma GCC diagnostic pop
#endif

