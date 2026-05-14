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

#define TRY(var, expr) \
	auto&& _tmp_##var = (expr); \
	if (!_tmp_##var) return std::unexpected{XfbinError::from(_tmp_##var.error())}; \
	auto var = *_tmp_##var

auto XfbinReader::parse_header()
	-> std::expected<void, XfbinError>
{
	TRY(file_signature, data.read<str>(4));
	if (file_signature != Xfbin::FILE_SIGNATURE) {
		return std::unexpected{
			XfbinError::MismatchedFileSignature{file_signature}
		};
	}

	TRY(version, data.read<u32>(std::endian::big));
	if (version != Xfbin::EXPECTED_VERSION) {
		return std::unexpected{
			XfbinError::MismatchedVersion{version}
		};
	}

	TRY(is_encrypted, data.read<bool>(sizeof(u16)));
	data.change_pos(sizeof(u16) * 3); // Skip misc flags.

	/* [TODO] Activate the decryptor. */

	return {};
}

auto XfbinReader::parse_index()
	-> std::expected<void, XfbinError>
{
	return {};
}

auto XfbinReader::parse_chunks()
	-> std::expected<void, XfbinError>
{
	return {};
}
