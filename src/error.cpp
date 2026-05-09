#include <xfbin/detail/error.hpp>

using namespace kojo;

XfbinError XfbinError::from(BinaryError err)
{
	return std::visit(overloaded{
		[](const auto& err) {
			return XfbinError::Unrecognized{err.to_string()};
		}
	}, err.variant);
}
