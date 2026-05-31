#include <xfbin/detail/xfbin_reader.hpp>

using namespace kojo;
using namespace kojo::nucc;
using namespace kojo::type_abbreviations;

auto Xfbin::from(const std::filesystem::path& path)
	-> std::expected<Xfbin, XfbinError>
{
        auto maybe_data = Binary::from(path);
        if (!maybe_data) {
		return std::unexpected{
			XfbinError::from(maybe_data.error().variant)
		};
        }
	return XfbinReader{*maybe_data}.parse();
}

auto Xfbin::from(std::span<const std::byte> span)
	-> std::expected<Xfbin, XfbinError>
{
	return XfbinReader{span}.parse();
}

auto Xfbin::from(const std::byte* ptr)
	-> std::expected<Xfbin, XfbinError>
{
	return XfbinReader{ptr}.parse();
}

auto Xfbin::fetch_type_from_map_index(std::uint32_t map_index) noexcept
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

auto Xfbin::fetch_path_from_map_index(std::uint32_t map_index) noexcept
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

auto Xfbin::fetch_name_from_map_index(std::uint32_t map_index) noexcept
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

auto Xfbin::add_page() -> Page&
{
	return pages.emplace_back();
}

auto Xfbin::add_page(const std::uint32_t chunk_map_offset, const std::uint32_t extra_map_offset)
	-> Page&
{
	return pages.emplace_back(chunk_map_offset, extra_map_offset);
}

// auto Xfbin::read_index(kojo::binary_view& data)
// -> std::expected<void, Error>
// {
// 	const auto error_return = std::unexpected{Error::cut_short};
//
// 	if (m_decryptor.should_decrypt) {
// 		u8* data_ptr = (u8*)data.data() + data.get_pos();
// 		m_decryptor.crypt(data_ptr, 52);
//         }
//
// 	data.change_pos(CHUNK_HEADER_SIZE); // Useless data that exists fsr
//
// 	struct CountsAndSizes {
// 		u32 type_count;
// 		u32 type_size;
// 		u32 path_count;
// 		u32 path_size;
// 		u32 name_count;
// 		u32 name_size;
// 		u32 map_count;
// 		u32 map_size;
// 		u32 map_indices_count;
// 		u32 extra_indices_count;
// 	};
// 	auto ints_buffer = data.read_struct<CountsAndSizes>();
// 	if (!ints_buffer) {
// 		return error_return;
// 	}
// 	auto ints = *ints_buffer;
//
// 	if (m_decryptor.should_decrypt) {
//                 u8* data_ptr = (u8*)data.data() + data.get_pos();
//
//                 m_decryptor.crypt(data_ptr, ints.type_size);
//                 data_ptr += ints.type_size;
//
//                 m_decryptor.crypt(data_ptr, ints.path_size);
//                 data_ptr += ints.path_size;
//
//                 m_decryptor.crypt(data_ptr, ints.name_size);
//                 data_ptr += ints.name_size;
//
//                 // Align data_ptr to 4 bytes.
//                 data_ptr += 4 - ((((u64)data_ptr - (u64)data.data()) - 1) % 4) - 1;
//
//                 m_decryptor.crypt(data_ptr, ints.map_size);
//                 data_ptr += ints.map_size;
//
//                 if (ints.extra_indices_count > 0) {
//                         m_decryptor.crypt(data_ptr, ints.extra_indices_count * 8);
//                         data_ptr += ints.extra_indices_count * 8;
//                 }
//
//                 if (ints.map_indices_count > 0) {
//                         m_decryptor.crypt(data_ptr, ints.map_indices_count * 4);
//                 }
//         }
//
// 	m_types.reserve(ints.type_count);
// 	for (size_t i = 0; i < ints.type_count; i++) {
// 		auto buffer = data.read<sv>();
// 		if (!buffer) {
// 			return error_return;
// 		}
//                 m_types.emplace_back(*buffer);
// 	}
//
// 	m_paths.reserve(ints.path_count);
//         for (size_t i = 0; i < ints.path_count; i++) {
//                 auto buffer = data.read<sv>();
// 		if (!buffer) {
// 			return error_return;
// 		}
//                 m_paths.emplace_back(*buffer);
// 	}
//
// 	m_names.reserve(ints.name_count);
//         for (size_t i = 0; i < ints.name_count; i++) {
//                 auto buffer = data.read<sv>();
// 		if (!buffer) {
// 			return error_return;
// 		}
//                 m_names.emplace_back(*buffer);
// 	}
//
//         data.align_by(4);
//
// 	m_maps.reserve(ints.map_count);
//         for (int i = 0; i < ints.map_count; i++) {
// 		auto buffer = data.read_struct<chunk_map>();
// 		if (!buffer) {
// 			return error_return;
// 		}
//                 m_maps.emplace_back(*buffer);
// 	}
//
// 	m_extra_indices.reserve(ints.extra_indices_count);
//         for (int i = 0; i < ints.extra_indices_count; i++) {
//                 auto buffer = data.read_struct<extra_indices>();
// 		if (!buffer) {
// 			return error_return;
// 		}
//                 m_extra_indices.emplace_back(*buffer);
// 	}
//
// 	m_map_indices.reserve(ints.map_indices_count);
//         for (int i = 0; i < ints.map_indices_count; i++) {
//                 auto buffer = data.read<u32>(std::endian::big);
// 		if (!buffer) {
// 			return error_return;
// 		}
//                 m_map_indices.emplace_back(*buffer);
// 	}
//
// 	return {};
// }
