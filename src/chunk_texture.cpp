#include <xfbin/chunk_texture.hpp>

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

auto ChunkTexture::from(const Chunk& chunk) noexcept
	-> std::expected<ChunkTexture, XfbinError>
{
	if (chunk.type != ChunkType::Texture) {
		return std::unexpected{
			XfbinError::MismatchedChunkType{ChunkType::Texture, chunk.type}
		};
	}

	ChunkTexture result;

	BinaryView reader{chunk.data};
	result.unk00  = TRY(reader.read<u16>(std::endian::big));
	result.width  = TRY(reader.read<u16>(std::endian::big));
	result.height = TRY(reader.read<u16>(std::endian::big));
	result.unk06  = TRY(reader.read<u16>(std::endian::big));
	result.size   = TRY(reader.read<u32>(std::endian::big));
	// [TODO] This *can* throw an exception. Need to wrap in my binary lib.
	result.data = std::span<const std::byte>{reader.get_pos_data(), result.size};

	return result;
}

#ifdef __clang__
#	pragma clang diagnostic pop
#elifdef __GNUC__
#	pragma GCC diagnostic pop
#endif
