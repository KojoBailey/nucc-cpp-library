#ifndef KOJO_XFBIN_HPP
#define KOJO_XFBIN_HPP

#include <kojo/binary.hpp>

#include <expected>
#include <filesystem>
#include <string>

namespace kojo {

class xfbin {
public:
	xfbin() = default;
	~xfbin() = default;

	/* Error handling */

	enum class error {
		ok,
		null_file,
	};

	[[nodiscard]] inline static constexpr std::string_view error_string(error code)
	{
		switch (code) {
		case error::ok:
			return "No errors detected.";
		case error::null_file:
			return "File does not exist.";
		}

		return "Unknown error code.";
	}

	/* Loading */

	[[nodiscard]] static auto load(const std::filesystem::path& path) -> std::expected<xfbin, error>;
};

}

#endif