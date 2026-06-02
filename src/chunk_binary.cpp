#include <xfbin/chunk_binary.hpp>

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

auto ChunkBinary::from(const Chunk& chunk) noexcept
	-> std::expected<ChunkBinary, XfbinError>
{
	if (chunk.type != ChunkType::Binary) {
		return std::unexpected{
			XfbinError::MismatchedChunkType{ChunkType::Binary, chunk.type}
		};
	}

	ChunkBinary result;

	BinaryView reader{chunk.data};
	result.size = TRY(reader.read<u32>(std::endian::big));
	// [TODO] This *can* throw an exception. Need to wrap in my binary lib.
	result.data = std::span<const std::byte>{reader.get_pos_data(), result.size};

	return result;
}

#ifdef __clang__
#	pragma clang diagnostic pop
#elifdef __GNUC__
#	pragma GCC diagnostic pop
#endif
