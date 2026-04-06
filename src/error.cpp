#include <xfbin/error.hpp>

using namespace kojo;

XfbinError XfbinError::create_null_file_error()
{
	XfbinError result;
	result.code = Code::NULL_FILE;
	return result;
}

XfbinError XfbinError::create_cut_short_error()
{
	XfbinError result;
	result.code = Code::CUT_SHORT;
	return result;
}

XfbinError XfbinError::create_mismatched_file_signature_error(
	std::uint32_t given_file_signature
)
{
	XfbinError result;
	result.code = Code::MISMATCHED_FILE_SIGNATURE;
	result.data.mismatched_file_signature = { given_file_signature };
	return result;
}

XfbinError XfbinError::create_mismatched_version_error(
	std::uint32_t given_version
)
{
	XfbinError result;
	result.code = Code::MISMATCHED_VERSION;
	result.data.mismatched_version = { given_version };
	return result;
}

XfbinError::Code XfbinError::get_code() const
{
	return code;
}

int XfbinError::get_code_as_int() const
{
	return static_cast<int>(code);
}

XfbinError::Data::MismatchedFileSignature XfbinError::get_mismatched_file_signature_data() const
{
	if (code != Code::MISMATCHED_FILE_SIGNATURE)
		throw std::bad_variant_access{};
	return data.mismatched_file_signature;
}

XfbinError::Data::MismatchedVersion XfbinError::get_mismatched_version_data() const
{
	if (code != Code::MISMATCHED_VERSION)
		throw std::bad_variant_access{};
	return data.mismatched_version;
}
