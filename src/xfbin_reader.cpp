#include <xfbin/detail/xfbin_reader.hpp>

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

auto XfbinReader::parse_header()
	-> std::expected<void, XfbinError>
{
	auto maybe_file_signature = data.read<str>(4);
	if (!maybe_file_signature) {
		return std::unexpected{
			XfbinError::from(maybe_file_signature.error())
		};
	}
	str& file_signature = *maybe_file_signature;
	if (file_signature != Xfbin::FILE_SIGNATURE) {
		return std::unexpected{
			XfbinError::MismatchedFileSignature{file_signature}
		};
	}

	auto maybe_version = data.read<u32>(std::endian::big);
	if (!maybe_version) {
		return std::unexpected{
			XfbinError::from(maybe_file_signature.error())
		};
	}
	u32 version = *maybe_version; 
	if (version != Xfbin::EXPECTED_VERSION) {
		return std::unexpected{
			XfbinError::MismatchedVersion{version}
		};
	}

	auto maybe_is_encrypted = data.read<bool>(sizeof(u16)); // [TODO] Implement BinaryView::read<bool>()
	data.change_pos(sizeof(u16) * 3); // Skip misc flags.
	if (!maybe_is_encrypted) {
		return std::unexpected{
			XfbinError::from(maybe_is_encrypted.error())
		};
	}
	bool is_encrypted = *maybe_file_signature;
	/* [TODO] Activate the decryptor. */

	return {};
}

auto XfbinReader::parse_index()
	-> std::expected<void, XfbinError>
{
}

auto XfbinReader::parse_chunks()
	-> std::expected<void, XfbinError>
{
}
