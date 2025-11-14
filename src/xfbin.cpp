#include <xfbin/xfbin.hpp>

using namespace kojo::binary_types;

namespace kojo {

auto xfbin::load(const std::filesystem::path& path)
-> std::expected<xfbin, error>
{
	xfbin result;

        result.m_decryptor.reset_state();

        auto data_buffer = kojo::binary::load(path);
        if (!data_buffer) {
		// !TODO - elaborate on error cases
                return std::unexpected{error::null_file};
        }
	kojo::binary data = *data_buffer;

        result.m_size = data.size();
        result.filename = path.stem().string();

        auto error_check = result.read(data);
	if (!error_check) {
		return std::unexpected{error_check.error()};
	}

	return result;
}

auto xfbin::read(kojo::binary_view data)
-> std::expected<void, error>
{
	return read_header(data)
		.and_then([&](auto&&) -> std::expected<void, error> { return read_index(data); })
		.and_then([&](auto&&) -> std::expected<void, error> { return read_chunks(data); });
}

auto xfbin::read_header(kojo::binary_view& data)
-> std::expected<void, error>
{
	auto magic_input = data.read<str>(4);
        if (magic_input != FILE_SIGNATURE) {
                return std::unexpected{error::file_signature};
        }

        auto version_input = data.read<u32>(std::endian::big);
        if (version_input != EXPECTED_VERSION) {
		return std::unexpected{error::version};
        }

	auto is_encrypted = static_cast<bool>(data.read<u16>(std::endian::big));
        data.change_pos(6); // Misc flags

        if (is_encrypted) {
                m_decryptor.should_decrypt = true;
        }

	return {};
}

auto xfbin::read_index(kojo::binary_view& data)
-> std::expected<void, error>
{
	const auto error_return = std::unexpected{error::cut_short};

	if (m_decryptor.should_decrypt) {
		u8* data_ptr = (u8*)data.data() + data.get_pos();
		m_decryptor.crypt(data_ptr, 52);
        }

	data.change_pos(CHUNK_HEADER_SIZE); // Useless data that exists fsr

	struct CountsAndSizes {
		u32 type_count;
		u32 type_size;
		u32 path_count;
		u32 path_size;
		u32 name_count;
		u32 name_size;
		u32 map_count;
		u32 map_size;
		u32 map_indices_count;
		u32 extra_indices_count;
	};
	auto ints_buffer = data.read_struct<CountsAndSizes>();
	if (!ints_buffer) {
		return error_return;
	}
	auto ints = *ints_buffer;

	if (m_decryptor.should_decrypt) {
                u8* data_ptr = (u8*)data.data() + data.get_pos();

                m_decryptor.crypt(data_ptr, ints.type_size);
                data_ptr += ints.type_size;

                m_decryptor.crypt(data_ptr, ints.path_size);
                data_ptr += ints.path_size;

                m_decryptor.crypt(data_ptr, ints.name_size);
                data_ptr += ints.name_size;

                // Align data_ptr to 4 bytes.
                data_ptr += 4 - ((((u64)data_ptr - (u64)data.data()) - 1) % 4) - 1;

                m_decryptor.crypt(data_ptr, ints.map_size);
                data_ptr += ints.map_size;

                if (ints.extra_indices_count > 0) {
                        m_decryptor.crypt(data_ptr, ints.extra_indices_count * 8);
                        data_ptr += ints.extra_indices_count * 8;
                }

                if (ints.map_indices_count > 0) {
                        m_decryptor.crypt(data_ptr, ints.map_indices_count * 4);
                }
        }

	m_types.reserve(ints.type_count);
	for (size_t i = 0; i < ints.type_count; i++) {
		auto buffer = data.read<sv>();
		if (!buffer) {
			return error_return;
		}
                m_types.emplace_back(*buffer);
	}

	m_paths.reserve(ints.path_count);
        for (size_t i = 0; i < ints.path_count; i++) {
                auto buffer = data.read<sv>();
		if (!buffer) {
			return error_return;
		}
                m_paths.emplace_back(*buffer);
	}

	m_names.reserve(ints.name_count);
        for (size_t i = 0; i < ints.name_count; i++) {
                auto buffer = data.read<sv>();
		if (!buffer) {
			return error_return;
		}
                m_names.emplace_back(*buffer);
	}

        data.align_by(4);

	m_maps.reserve(ints.map_count);
        for (int i = 0; i < ints.map_count; i++) {
		auto buffer = data.read_struct<chunk_map>();
		if (!buffer) {
			return error_return;
		}
                m_maps.emplace_back(*buffer);
	}

	m_extra_indices.reserve(ints.extra_indices_count);
        for (int i = 0; i < ints.extra_indices_count; i++) {
                auto buffer = data.read_struct<extra_indices>();
		if (!buffer) {
			return error_return;
		}
                m_extra_indices.emplace_back(*buffer);
	}

	m_map_indices.reserve(ints.map_indices_count);
        for (int i = 0; i < ints.map_indices_count; i++) {
                auto buffer = data.read<u32>(std::endian::big);
		if (!buffer) {
			return error_return;
		}
                m_map_indices.emplace_back(*buffer);
	}

	return {};
}


auto xfbin::read_chunks(kojo::binary_view&)
-> std::expected<void, error>
{
	return {};
}

}