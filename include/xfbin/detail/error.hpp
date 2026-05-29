#ifndef KOJO_XFBIN_ERROR_HPP
#define KOJO_XFBIN_ERROR_HPP

#include <kojo/binary.hpp>

#include <cstdint>
#include <filesystem>
#include <format>
#include <string>
#include <variant>

namespace kojo::nucc {

struct XfbinError {
	template<typename T>
	XfbinError(T&& err) : variant(std::forward<T>(err)) {}

	struct Unrecognized {
		static const std::uint32_t code = 999;

		const std::string msg;

		Unrecognized() = delete;
		Unrecognized(const std::string _msg)
			: msg(_msg) {}

		std::string to_string() const {
			return msg;
		}
	};

	struct InsufficientMemory {
		static const std::uint32_t code = 0;

		const std::byte* address;
		const std::size_t size;

		InsufficientMemory() = delete;
		InsufficientMemory(const std::byte* _address, const std::size_t _size)
			: address(_address), size(_size) {}

		std::string to_string() const {
			return std::format("Failed to allocate sufficient memory to load the XFBIN. Address: {:08x}; Requested Size: {}.",
				reinterpret_cast<std::size_t>(address), size
			);
		}
	};

	struct UnexpectedEnd {
		static const std::uint32_t code = 1;

		std::string to_string() const {
			return "Reached unexpected end of XFBIN.";
		}
	};

	struct FileNotFound {
		static const std::uint32_t code = 2;

		const std::filesystem::path path;

		FileNotFound() = delete;
		FileNotFound(const std::filesystem::path _path)
			: path(_path) {}

		std::string to_string() const {
			return std::format("XFBIN at \"{}\" could not be found.", path.string());
		}
	};

	struct InvalidFile {
		static const std::uint32_t code = 3;

		const std::filesystem::path path;

		InvalidFile() = delete;
		InvalidFile(const std::filesystem::path _path)
			: path(_path) {}

		std::string to_string() const {
			return std::format("XFBIN at \"{}\" is not a valid file. It may be a directory instead.",
				path.string()
			);
		}
	};

	struct FileNotOpen {
		static const std::uint32_t code = 4;

		const std::filesystem::path path;

		FileNotOpen() = delete;
		FileNotOpen(const std::filesystem::path _path)
			: path(_path) {}

		std::string to_string() const {
			return std::format("XFBIN at \"{}\" failed to open.", path.string());
		}
	};
	
	struct MismatchedFileSignature {
		static const std::uint32_t code = 100;

		const std::string given_file_signature;

		MismatchedFileSignature() = delete;
		MismatchedFileSignature(const std::string _given_file_signature)
			: given_file_signature(_given_file_signature) {}

		std::string to_string() const {
			return std::format("Expected file signature `NUCC` (4E 55 43 43) but got `{}`.",
				given_file_signature
			);
		}
	};

	struct MismatchedVersion {
		static const std::uint32_t code = 101;

		const std::uint32_t given_version;

		MismatchedVersion() = delete;
		MismatchedVersion(const std::uint32_t _given_version)
			: given_version(_given_version) {}

		std::string to_string() const {
			return std::format("Expected XFBIN version 121 but got {}.",
				given_version
			);
		}
	};

	struct MapIndexOutOfBounds {
		static const std::uint32_t code = 200;

		const std::uint32_t given_index;
		const std::uint32_t max_index;

		MapIndexOutOfBounds() = delete;
		MapIndexOutOfBounds(const std::uint32_t _given_index, const std::size_t _max_index)
			: given_index(_given_index), max_index(_max_index) {}

		std::string to_string() const {
			return std::format("Map index of {} out of bounds for map indices size {}.",
				given_index, max_index
			);
		}
	};

	struct UnrecognizedChunkTypeString {
		static const std::uint32_t code = 201;

		const std::string given_chunk_type;

		UnrecognizedChunkTypeString() = delete;
		UnrecognizedChunkTypeString(std::string_view _given_chunk_type)
			: given_chunk_type(_given_chunk_type) {}
		
		std::string to_string() const {
			return std::format("Unrecognised chunk type: {}", given_chunk_type); 
		}
	};

	std::variant<
		Unrecognized,
		InsufficientMemory,
		UnexpectedEnd,
		FileNotFound,
		InvalidFile,
		FileNotOpen,
		MismatchedFileSignature,
		MismatchedVersion,
		MapIndexOutOfBounds,
		UnrecognizedChunkTypeString
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
