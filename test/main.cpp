#include <xfbin/error.hpp>

#include <iostream>
#include <expected>

using namespace kojo;

int main(int argc, char* argv[])
{
	std::expected<int, XfbinError> maybe_xfbin = std::unexpected{
		XfbinError::create_mismatched_version_error(122)
	};

	if (!maybe_xfbin) {
		auto err = maybe_xfbin.error();
		std::cerr << std::format("[ERR{:03}] ", err.get_code_as_int());
		switch (err.get_code()) {
		case XfbinError::Code::MISMATCHED_VERSION:
			std::cerr << std::format("Expected version 121 but got {}",
				err.get_mismatched_version_data().given_version);
			return 1;
		default:
			std::cerr << "Unknown";
			return 2;
		}
	}

	// if (argc < 2) {
	// 	std::cerr << "Too few arguments.\n";
	// 	return 1;
	// }
	//
	// auto xfbin_buffer = Xfbin::load(argv[1]);
	// if (!xfbin_buffer) {
	// 	std::cout << std::format("Error: {}\n", Xfbin::error_string(xfbin_buffer.error()));
	// 	return 1;
	// }
	// Xfbin xfbin_obj = *xfbin_buffer;
	//
	// std::cout << xfbin_obj.version();
}
