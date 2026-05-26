#include <xfbin/detail/xfbin_reader.hpp>
#include <xfbin/detail/index.hpp>

using namespace kojo;
using namespace kojo::type_abbreviations;

auto XfbinReader::parse() &&
	-> std::expected<Xfbin, XfbinError>
{
	return parse_header()
		.and_then([&] { return parse_index(); })
		.and_then([&] { return parse_chunks(); })
		.transform([&] { return std::move(result); });
}

/* This macro restricts compilation to GCC and Clang.
 * Otherwise usage would be `TRY(file_signature, data.read<str>(4));`
 */
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

	const auto sizes = TRY(data.read_struct<NuccIndexSizes>());

	for (std::uint32_t i = 0; i < sizes.chunk_type_count; i++) {
		const auto chunk_type = TRY(data.read<sv>());
		result.types.emplace_back(chunk_type);
	}

	for (std::uint32_t i = 0; i < sizes.file_path_count; i++) {
		const auto file_path = TRY(data.read<sv>());
		result.paths.emplace_back(file_path);
	}

	for (std::uint32_t i = 0; i < sizes.chunk_name_count; i++) {
		const auto chunk_name = TRY(data.read<sv>());
		result.names.emplace_back(chunk_name);
	}

	for (std::uint32_t i = 0; i < sizes.chunk_map_count; i++) {
		const auto chunk_map = TRY(data.read_struct<Xfbin::ChunkMap>());
		result.maps.push_back(chunk_map);
	}

	for (std::uint32_t i = 0; i < sizes.extra_map_indices_count; i++) {
		const auto extra_map_indices = TRY(data.read_struct<Xfbin::ExtraIndices>());
		result.extra_indices.push_back(extra_map_indices);
	}

	for (std::uint32_t i = 0; i < sizes.chunk_map_indices_count; i++) {
		const auto map_index = TRY(data.read<u32>(std::endian::big));
		result.map_indices.push_back(map_index);
	}

	return {};
}

auto XfbinReader::parse_chunks()
	-> std::expected<void, XfbinError>
{
	return {};
}
