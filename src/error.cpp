#include <xfbin/detail/error.hpp>

using namespace kojo;
using namespace kojo::nucc;

auto XfbinError::from(const XfbinError& err) -> XfbinError
{
	return err;
}

auto XfbinError::from(const BinaryError& err) -> XfbinError
{
	return std::visit(overloaded{
		[](const auto& err) -> XfbinError {
			return XfbinError::Unrecognized{err.to_string()};
		}
	}, err.variant);
}
