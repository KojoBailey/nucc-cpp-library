#ifndef KOJO_XFBIN_ERROR_HPP
#define KOJO_XFBIN_ERROR_HPP

#include <string>

namespace kojo {

enum class xfbin_error {
	ok,
	null_file,
};

[[nodiscard]] inline constexpr std::string_view xfbin_error_string(xfbin_error code)
{
	switch (code) {
	case xfbin_error::ok:
		return "No errors detected.";
	case xfbin_error::null_file:
		return "File does not exist.";
	}

	return "Unknown error code.";
}

}

#endif