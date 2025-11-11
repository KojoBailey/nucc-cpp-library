#include <xfbin/xfbin.hpp>

using namespace kojo::binary_types;

namespace kojo {

auto xfbin::load(const std::filesystem::path& path)
-> std::expected<xfbin, error>
{
	xfbin result;
        // cryptor.reset_state(); // Reset the cryptor state before loading a new file.

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
		.and_then([&](auto) { return read_index(data); })
		.and_then([&](auto) { return read_chunks(data); });
}

auto xfbin::read_header(kojo::binary_view& data)
-> std::expected<void, error>
{
	auto magic_input = data.read<str>(4);
        if (magic_input != MAGIC) {
                return std::unexpected{error::file_signature};
        }

        auto version_input = data.read<u32>(std::endian::big);
        if (version_input != VERSION) {
		return std::unexpected{error::version};
        }

        auto flags = data.read<u64>(std::endian::little); // !TODO Parse *all* of these at some point.

        // Extract the second-lowest byte from flags
        // uint8_t decryption_flag = (flags >> 8) & 0xFF;
        // if (decryption_flag != 0) {
        //         should_decrypt = true;
        // }

	return {};
}

}