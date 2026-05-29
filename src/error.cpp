#include <xfbin/detail/error.hpp>

using namespace kojo;
using namespace kojo::nucc;

XfbinError XfbinError::from(BinaryError err)
{
	return std::visit(overloaded{
		[](const auto& err) {
			return XfbinError::Unrecognized{err.to_string()};
		}
	}, err.variant);
}
