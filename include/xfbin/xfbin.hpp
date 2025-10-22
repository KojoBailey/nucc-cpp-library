#ifndef KOJO_XFBIN_HPP
#define KOJO_XFBIN_HPP

#include <xfbin/error.hpp>

#include <expected>
#include <filesystem>
#include <string>

namespace kojo {

class xfbin {
public:
	xfbin() = default;
	~xfbin() = default;

	[[nodiscard]] static auto load(const std::filesystem::path& path) -> std::expected<xfbin, xfbin_error>;
};

}

#endif