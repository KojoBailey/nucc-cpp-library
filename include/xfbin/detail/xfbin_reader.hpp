#include <xfbin/xfbin.hpp>

struct XfbinReader {
	BinaryView data;
	Xfbin result;

	XfbinReader(BinaryView _data) : data(_data) {}

	[[nodiscard]] auto parse() && -> std::expected<Xfbin, XfbinError>;
	[[nodiscard]] auto parse_header() -> std::expected<void, XfbinError>;
	[[nodiscard]] auto parse_index()  -> std::expected<void, XfbinError>;
	[[nodiscard]] auto parse_chunks() -> std::expected<void, XfbinError>;
};
