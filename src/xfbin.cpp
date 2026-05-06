#include <xfbin/xfbin.hpp>

using namespace kojo;
using namespace kojo::binary_types;

XfbinError binary_error_to_xfbin_error(kojo::binary::error err)
{
		switch (err) {
		case kojo::binary::error::file_not_exist:
			return XfbinError::create_null_file_error();
		case kojo::binary::error::invalid_file:
			return XfbinError::create_null_file_error();
		case kojo::binary::error::file_not_open:
			return XfbinError::create_null_file_error();
		case kojo::binary::error::insufficient_memory:
			return XfbinError::create_null_file_error();
		}
		return XfbinError::create_null_file_error();
}

auto Xfbin::read_from_path(const std::filesystem::path& path) -> std::expected<Xfbin, XfbinError>
{
        auto maybe_data = kojo::binary::load(path);
        if (!maybe_data) {
		return std::unexpected{
			binary_error_to_xfbin_error(maybe_data.error())
		};
        }
	kojo::binary data = maybe_data.value();

	return parse(data);
}

auto Xfbin::read_from_span(std::span<const std::byte> span) -> std::expected<Xfbin, XfbinError>
{
	return parse(span);
}

auto Xfbin::read_from_pointer(const std::byte* ptr) -> std::expected<Xfbin, XfbinError>
{
	return parse(ptr);
}

auto Xfbin::parse(kojo::binary_view data)
	-> std::expected<Xfbin, XfbinError>
{
	Xfbin result;
	result.parse_header(data)
		.and_then([&]() { return result.parse_index(data); })
		.and_then([&]() { return result.parse_chunks(data); });
	return result;
}
//
// auto Xfbin::read_header(kojo::binary_view& data)
// -> std::expected<void, Error>
// {
// 	// [TODO] Handle binary_view::read errors properly
// 	auto magic_input_buffer = data.read<str>(4);
// 	if (!magic_input_buffer) {
// 		return std::unexpected{Error::cut_short};
// 	}
// 	std::string magic_input = *magic_input_buffer;
//         if (magic_input != FILE_SIGNATURE) {
//                 return std::unexpected{Error::file_signature};
//         }
//
//         auto version_input = data.read<u32>(std::endian::big);
//         if (!version_input || *version_input != EXPECTED_VERSION) {
// 		return std::unexpected{Error::version};
//         }
//
// 	auto is_encrypted = static_cast<bool>(data.read<u16>(std::endian::big));
//         data.change_pos(6); // Misc flags
//
//         if (is_encrypted) {
//                 m_decryptor.should_decrypt = true;
//         }
//
// 	return {};
// }
//
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
//
// auto Xfbin::read_chunks(kojo::binary_view&)
// -> std::expected<void, Error>
// {
// 	// !TODO
// 	return {};
// }
