#include <xfbin/detail/xfbin_reader.hpp>

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
}

auto XfbinReader::parse_index()
	-> std::expected<void, XfbinError>
{
}

auto XfbinReader::parse_chunks()
	-> std::expected<void, XfbinError>
{
}
