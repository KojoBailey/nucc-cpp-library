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
	auto file_signature = TRY(data.read<str>(4));
	if (file_signature != Xfbin::FILE_SIGNATURE) {
		return std::unexpected{
			XfbinError::MismatchedFileSignature{file_signature}
		};
	}

	auto version = TRY(data.read<u32>(std::endian::big));
	if (version != Xfbin::EXPECTED_VERSION) {
		return std::unexpected{
			XfbinError::MismatchedVersion{version}
		};
	}

	auto is_encrypted = TRY(data.read<bool>(sizeof(u16)));
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
