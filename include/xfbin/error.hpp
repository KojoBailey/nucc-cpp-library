#include <cstdint>
#include <stdexcept>
#include <variant>

namespace kojo {

struct XfbinError {
	enum class Code {
		NULL_FILE                 = 000,
		CUT_SHORT                 = 001,
		MISMATCHED_FILE_SIGNATURE = 100,
		MISMATCHED_VERSION        = 101,
	} code;

	union Data {
		struct MismatchedFileSignature {
			std::uint32_t given_file_signature;
		} mismatched_file_signature;

		struct MismatchedVersion {
			std::uint32_t given_version;
		} mismatched_version;
	} data;

	[[nodiscard]] static XfbinError create_null_file_error();
	[[nodiscard]] static XfbinError create_cut_short_error();
	[[nodiscard]] static XfbinError create_mismatched_file_signature_error(
		std::uint32_t given_file_signature
	);
	[[nodiscard]] static XfbinError create_mismatched_version_error(
		std::uint32_t given_version
	);

	[[nodiscard]] int code_as_int() const;

	[[nodiscard]] Data::MismatchedFileSignature get_mismatched_file_signature_data() const;
	[[nodiscard]] Data::MismatchedVersion get_mismatched_version_data() const;
};

}
