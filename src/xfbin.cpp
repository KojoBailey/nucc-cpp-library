#include <xfbin/xfbin.hpp>

namespace kojo {

auto xfbin::load(const std::filesystem::path& path)
-> std::expected<xfbin, error>
{
	return std::unexpected{error::null_file};
}

}