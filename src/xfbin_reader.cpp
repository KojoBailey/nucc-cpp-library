#include <xfbin/detail/xfbin_reader.hpp>
#include <xfbin/detail/index.hpp>

using namespace kojo;
using namespace kojo::nucc;
using namespace kojo::type_abbreviations;

auto XfbinReader::parse() &&
	-> std::expected<Xfbin, XfbinError>
{
	return parse_header()
		.and_then([&] { return parse_index(); })
		.and_then([&] { return parse_chunks(); })
		.transform([&] { return result; });
}

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

auto XfbinReader::parse_header()
	-> std::expected<void, XfbinError>
{
	const auto file_signature = TRY(data.read<str>(4));
	if (file_signature != Xfbin::FILE_SIGNATURE) {
		return std::unexpected{
			XfbinError::MismatchedFileSignature{file_signature}
		};
	}

	const auto version = TRY(data.read<u32>(std::endian::big));
	if (version != Xfbin::EXPECTED_VERSION) {
		return std::unexpected{
			XfbinError::MismatchedVersion{version}
		};
	}

	const auto is_encrypted = TRY(data.read<bool>(sizeof(u16)));
	data.change_pos(sizeof(u16) * 3); // Skip misc flags.

	/* [TODO] Activate the decryptor. */

	return {};
}

auto XfbinReader::parse_index()
	-> std::expected<void, XfbinError>
{
	data.change_pos(CHUNK_HEADER_SIZE); // Useless metadata.

	NuccIndexSizes sizes;
	sizes.chunk_type_count        = TRY(data.read<u32>(std::endian::big));
	data.change_pos(sizeof(u32));
	sizes.file_path_count         = TRY(data.read<u32>(std::endian::big));
	data.change_pos(sizeof(u32));
	sizes.chunk_name_count        = TRY(data.read<u32>(std::endian::big));
	data.change_pos(sizeof(u32));
	sizes.chunk_map_count         = TRY(data.read<u32>(std::endian::big));
	data.change_pos(sizeof(u32));
	sizes.chunk_map_indices_count = TRY(data.read<u32>(std::endian::big));
	sizes.extra_map_indices_count = TRY(data.read<u32>(std::endian::big));

	result.maps.reserve(sizes.chunk_type_count);
	for (u32 i = 0; i < sizes.chunk_type_count; i++) {
		const auto chunk_type_str = TRY(data.read<sv>());
		const auto maybe_chunk_type = chunk_type_from_string(chunk_type_str);
		if (!maybe_chunk_type) {
			return std::unexpected{
				XfbinError::UnrecognizedChunkTypeString{chunk_type_str}
			};
		}
		const auto chunk_type = *maybe_chunk_type;
		result.types.emplace_back(chunk_type);
	}

	result.maps.reserve(sizes.file_path_count);
	for (u32 i = 0; i < sizes.file_path_count; i++) {
		const auto file_path = TRY(data.read<sv>());
		result.paths.emplace_back(file_path);
	}

	result.maps.reserve(sizes.chunk_name_count);
	for (u32 i = 0; i < sizes.chunk_name_count; i++) {
		const auto chunk_name = TRY(data.read<sv>());
		result.names.emplace_back(chunk_name);
	}

	data.align_by(4);

	result.maps.reserve(sizes.chunk_map_count);
	for (u32 i = 0; i < sizes.chunk_map_count; i++) {
		const auto type_index = TRY(data.read<u32>(std::endian::big));
		const auto path_index = TRY(data.read<u32>(std::endian::big));
		const auto name_index = TRY(data.read<u32>(std::endian::big));
		result.maps.emplace_back(type_index, path_index, name_index);
	}

	result.maps.reserve(sizes.extra_map_indices_count);
	for (u32 i = 0; i < sizes.extra_map_indices_count; i++) {
		const auto name_index = TRY(data.read<u32>(std::endian::big));
		const auto map_index = TRY(data.read<u32>(std::endian::big));
		result.extra_indices.emplace_back(name_index, map_index);
	}

	result.maps.reserve(sizes.chunk_map_indices_count);
	for (u32 i = 0; i < sizes.chunk_map_indices_count; i++) {
		const auto map_index = TRY(data.read<u32>(std::endian::big));
		result.map_indices.push_back(map_index);
	}

	return {};
}

auto XfbinReader::parse_chunks()
	-> std::expected<void, XfbinError>
{
	result.add_page();

	for (std::size_t page = 0; !data.is_at_end();) {
		const auto chunk_size    = TRY(data.read<u32>(std::endian::big));
		const auto map_index     = TRY(data.read<u32>(std::endian::big));
		const auto chunk_version = TRY(data.read<u16>(std::endian::big));
		const auto unk           = TRY(data.read<u16>(std::endian::big));

		const auto chunk_type = *result.fetch_type_from_map_index(map_index);
		const auto chunk_path = *result.fetch_path_from_map_index(map_index);
		const auto chunk_name = *result.fetch_name_from_map_index(map_index);

		if (chunk_type == ChunkType::Page) {
			const auto chunk_map_offset = TRY(data.read<u32>(std::endian::big));
			const auto extra_map_offset = TRY(data.read<u32>(std::endian::big));
			result.add_page(chunk_map_offset, extra_map_offset);
			page++;
		} else {
			const auto chunk_data = TRY(Binary::from(data, chunk_size, data.get_pos()));

			result.pages[page].add_chunk(chunk_type, chunk_path, chunk_name,
				chunk_version, unk, chunk_data);

			data.change_pos(chunk_size);
		}
	}

	return {};
}

#ifdef __clang__
#	pragma clang diagnostic pop
#elifdef __GNUC__
#	pragma GCC diagnostic pop
#endif
