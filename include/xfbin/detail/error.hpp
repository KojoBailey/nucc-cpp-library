#ifndef KOJO_XFBIN_ERROR_HPP
#define KOJO_XFBIN_ERROR_HPP

#include <kojo/binary.hpp>

#include <cstdint>
#include <filesystem>
#include <format>
#include <string>
#include <variant>

namespace kojo {

struct XfbinError {
	template<typename T>
	XfbinError(T&& err) : variant(std::forward<T>(err)) {}

	struct Unrecognized {
		static const std::uint32_t code = 999;

		std::string to_string() const {
			return msg;
		}

		Unrecognized() = delete;
		Unrecognized(const std::string _msg)
			: msg(_msg) {}

		const std::string msg;
	};

	struct InsufficientMemory {
		static const std::uint32_t code = 0;

		std::string to_string() const {
			return std::format("Failed to allocate sufficient memory to load the XFBIN. Address: {:08x}; Requested Size: {}.",
				reinterpret_cast<std::size_t>(address), size
			);
		}

		InsufficientMemory() = delete;
		InsufficientMemory(const std::byte* _address, const std::size_t _size)
			: address(_address), size(_size) {}

		const std::byte* address;
		const std::size_t size;
	};

	struct UnexpectedEnd {
		static const std::uint32_t code = 1;

		std::string to_string() const {
			return "Reached unexpected end of XFBIN.";
		}
	};

	struct FileNotFound {
		static const std::uint32_t code = 2;

		std::string to_string() const {
			return std::format("XFBIN at \"{}\" could not be found.", path.string());
		}

		FileNotFound() = delete;
		FileNotFound(const std::filesystem::path _path)
			: path(_path) {}

		const std::filesystem::path path;
	};

	struct InvalidFile {
		static const std::uint32_t code = 3;

		std::string to_string() const {
			return std::format("XFBIN at \"{}\" is not a valid file. It may be a directory instead.",
				path.string()
			);
		}

		InvalidFile() = delete;
		InvalidFile(const std::filesystem::path _path)
			: path(_path) {}

		const std::filesystem::path path;
	};

	struct FileNotOpen {
		static const std::uint32_t code = 4;

		std::string to_string() const {
			return std::format("XFBIN at \"{}\" failed to open.", path.string());
		}

		FileNotOpen() = delete;
		FileNotOpen(const std::filesystem::path _path)
			: path(_path) {}

		const std::filesystem::path path;
	};
	
	struct MismatchedFileSignature {
		static const std::uint32_t code = 100;

		std::string to_string() const {
			return std::format("Expected file signature `NUCC` (4E 55 43 43) but got `{}`.",
				given_file_signature
			);
		}

		MismatchedFileSignature() = delete;
		MismatchedFileSignature(const std::string _given_file_signature)
			: given_file_signature(_given_file_signature) {}

		const std::string given_file_signature;
	};

	struct MismatchedVersion {
		static const std::uint32_t code = 101;

		std::string to_string() const {
			return std::format("Expected XFBIN version 121 but got {}.",
				given_version
			);
		}

		MismatchedVersion() = delete;
		MismatchedVersion(const std::uint32_t _given_version)
			: given_version(_given_version) {}

		const std::uint32_t given_version;
	};

	std::variant<
		Unrecognized,
		InsufficientMemory,
		UnexpectedEnd,
		FileNotFound,
		InvalidFile,
		FileNotOpen,
		MismatchedFileSignature,
		MismatchedVersion
	> variant;

	std::uint32_t to_code() const {
		return std::visit([](const auto& err) { return err.code; }, variant);
	}

	std::string to_string() const {
		return std::visit([](const auto& err) { return err.to_string(); }, variant);
	}

	static XfbinError from(BinaryError err);
};

}

#endif
