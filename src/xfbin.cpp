#include <xfbin/detail/xfbin_reader.hpp>

using namespace kojo;
using namespace kojo::nucc;
using namespace kojo::type_abbreviations;

auto Xfbin::from(const std::filesystem::path& path, const std::array<u8, 8> crypt_key)
	-> std::expected<Xfbin, XfbinError>
{
        auto maybe_data = Binary::from(path);
        if (!maybe_data) {
		return std::unexpected{
			XfbinError::from(maybe_data.error().variant)
		};
        }
	return XfbinReader{*maybe_data, crypt_key}.parse();
}

auto Xfbin::from(std::span<const std::byte> span, const std::array<u8, 8> crypt_key)
	-> std::expected<Xfbin, XfbinError>
{
	return XfbinReader{span, crypt_key}.parse();
}

auto Xfbin::from(const std::byte* ptr, const std::array<u8, 8> crypt_key)
	-> std::expected<Xfbin, XfbinError>
{
	return XfbinReader{ptr, crypt_key}.parse();
}

auto Xfbin::fetch_type_from_map_index(std::uint32_t map_index) const noexcept
	-> std::expected<ChunkType, XfbinError>
{
	if (map_index >= map_indices.size()) {
		return std::unexpected{
			XfbinError::MapIndexOutOfBounds{map_index, map_indices.size()}
		};
	}

	const std::uint32_t true_map_index = map_indices[map_index];
		
	if (true_map_index >= maps.size()) {
		return std::unexpected{
			XfbinError::MapIndexOutOfBounds{true_map_index, maps.size()}
		};
	}
		
	const ChunkMap& chunk_map = maps[true_map_index];
	return types[chunk_map.type_index];
}

auto Xfbin::fetch_path_from_map_index(std::uint32_t map_index) const noexcept
	-> std::expected<std::string_view, XfbinError>
{
	if (map_index >= map_indices.size()) {
		return std::unexpected{
			XfbinError::MapIndexOutOfBounds{map_index, map_indices.size()}
		};
	}

	const std::uint32_t true_map_index = map_indices[map_index];
		
	if (true_map_index >= maps.size()) {
		return std::unexpected{
			XfbinError::MapIndexOutOfBounds{true_map_index, maps.size()}
		};
	}
		
	const ChunkMap& chunk_map = maps[true_map_index];
	return paths[chunk_map.path_index];
}

auto Xfbin::fetch_name_from_map_index(std::uint32_t map_index) const noexcept
	-> std::expected<std::string_view, XfbinError>
{
	if (map_index >= map_indices.size()) {
		return std::unexpected{
			XfbinError::MapIndexOutOfBounds{map_index, map_indices.size()}
		};
	}

	const std::uint32_t true_map_index = map_indices[map_index];
		
	if (true_map_index >= maps.size()) {
		return std::unexpected{
			XfbinError::MapIndexOutOfBounds{true_map_index, maps.size()}
		};
	}
		
	const ChunkMap& chunk_map = maps[true_map_index];
	return names[chunk_map.name_index];
}

auto Xfbin::fetch_chunk(
	std::optional<ChunkType> type,
	std::optional<std::string_view> path,
	std::optional<std::string_view> name,
	std::size_t index
) -> Chunk*
{
	std::size_t i = 0;
	for (auto& page : pages) {
		for (auto& chunk : page.chunks) {
			if (type.has_value() && chunk.type != type) {
				continue;
			}
			if (path.has_value() && chunk.path != path) {
				continue;
			}
			if (name.has_value() && chunk.name != name) {
				continue;
			}

			if (i == index) {
				return &chunk;
			}
			i++;
		}
	}

	return nullptr;
}

auto Xfbin::fetch_chunk(
	std::optional<ChunkType> type,
	std::optional<std::string_view> path,
	std::optional<std::string_view> name,
	std::size_t index
) const -> const Chunk*
{
	std::size_t i = 0;
	for (const auto& page : pages) {
		for (const auto& chunk : page.chunks) {
			if (type.has_value() && chunk.type != type) {
				continue;
			}
			if (path.has_value() && chunk.path != path) {
				continue;
			}
			if (name.has_value() && chunk.name != name) {
				continue;
			}

			if (i == index) {
				return &chunk;
			}
			i++;
		}
	}

	return nullptr;
}

auto Xfbin::add_page() -> Page&
{
	return pages.emplace_back();
}

auto Xfbin::add_page(const std::uint32_t chunk_map_offset, const std::uint32_t extra_map_offset)
	-> Page&
{
	return pages.emplace_back(chunk_map_offset, extra_map_offset);
}
