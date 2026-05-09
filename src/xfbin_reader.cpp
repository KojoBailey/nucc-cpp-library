#include <xfbin/detail/xfbin_reader.hpp>

using namespace kojo;
using namespace kojo::type_abbreviations;

auto XfbinReader::parse()
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
	std::string& file_signature = *maybe_file_signature;
	if (file_signature != FILE_SIGNATURE) {
		return std::unexpected{
			XfbinError::MismatchedFileSignature{file_signature};
		};
	}
}

auto XfbinReader::parse_index()
	-> std::expected<void, XfbinError>
{
}

auto XfbinReader::parse_chunks()
	-> std::expected<void, XfbinError>
{
}
