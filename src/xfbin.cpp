#include <xfbin/xfbin.hpp>

namespace kojo {

auto xfbin::load(const std::filesystem::path& path) -> std::expected<xfbin, xfbin_error>
{
	return std::unexpected{xfbin_error::null_file};
}

}