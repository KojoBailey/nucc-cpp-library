#ifndef KOJO_XFBIN_ERROR_HPP
#define KOJO_XFBIN_ERROR_HPP

#include <xfbin/chunk_type.hpp>

#include <kojo/binary.hpp>

#include <cstdint>
#include <filesystem>
#include <format>
#include <string>
#include <variant>

namespace kojo::nucc {

struct XfbinError {
	template<typename T>
	XfbinError(T&& err) : variant{std::forward<T>(err)} {}

	struct Unrecognized {
		static const std::uint32_t code = 999;

		const std::string msg;

		Unrecognized() = delete;
		Unrecognized(std::string _msg)
			: msg{std::move(_msg)} {}

		[[nodiscard]] auto to_string() const -> std::string {
			return msg;
		}
	};

	struct InsufficientMemory {
		static const std::uint32_t code = 0;

		const std::byte* address;
		const std::size_t size;

		InsufficientMemory() = delete;
		InsufficientMemory(const std::byte* _address, std::size_t _size)
			: address{_address}, size{_size} {}

		[[nodiscard]] auto to_string() const -> std::string {
			return std::format("Failed to allocate sufficient memory to load the XFBIN. Address: {:08x}; Requested Size: {}.",
				reinterpret_cast<std::size_t>(address), size
			);
		}
	};

	struct UnexpectedEnd {
		static const std::uint32_t code = 1;

		[[nodiscard]] static auto to_string() -> std::string {
			return "Reached unexpected end of XFBIN.";
		}
	};

	struct FileNotFound {
		static const std::uint32_t code = 2;

		const std::filesystem::path path;

		FileNotFound() = delete;
		FileNotFound(std::filesystem::path _path)
			: path{std::move(_path)} {}

		[[nodiscard]] auto to_string() const -> std::string {
			return std::format("XFBIN at \"{}\" could not be found.", path.string());
		}
	};

	struct InvalidFile {
		static const std::uint32_t code = 3;

		const std::filesystem::path path;

		InvalidFile() = delete;
		InvalidFile(std::filesystem::path _path)
			: path{std::move(_path)} {}

		[[nodiscard]] auto to_string() const -> std::string {
			return std::format("XFBIN at \"{}\" is not a valid file. It may be a directory instead.",
				path.string()
			);
		}
	};

	struct FileNotOpen {
		static const std::uint32_t code = 4;

		const std::filesystem::path path;

		FileNotOpen() = delete;
		FileNotOpen(std::filesystem::path _path)
			: path{std::move(_path)} {}

		[[nodiscard]] auto to_string() const -> std::string {
			return std::format("XFBIN at \"{}\" failed to open.", path.string());
		}
	};
	
	struct MismatchedFileSignature {
		static const std::uint32_t code = 100;

		const std::string given_file_signature;

		MismatchedFileSignature() = delete;
		MismatchedFileSignature(std::string _given_file_signature)
			: given_file_signature{std::move(_given_file_signature)} {}

		[[nodiscard]] auto to_string() const -> std::string {
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
			: given_version{_given_version} {}

		[[nodiscard]] auto to_string() const -> std::string {
			return std::format("Expected XFBIN version 121 but got {}.",
				given_version
			);
		}
	};

	struct MapIndexOutOfBounds {
		static const std::uint32_t code = 200;

		const std::uint32_t given_index;
		const std::size_t max_index;

		MapIndexOutOfBounds() = delete;
		MapIndexOutOfBounds(const std::uint32_t _given_index, const std::size_t _max_index)
			: given_index{_given_index}, max_index{_max_index} {}

		[[nodiscard]] auto to_string() const -> std::string {
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
			: given_chunk_type{_given_chunk_type} {}
		
		[[nodiscard]] auto to_string() const -> std::string {
			return std::format("Unrecognised chunk type: {}.", given_chunk_type); 
		}
	};

	struct MismatchedChunkType {
		static const std::uint32_t code = 202; // [TODO] Maybe make code u16?

		const ChunkType expected_chunk_type;
		const ChunkType given_chunk_type;

		MismatchedChunkType() = delete;
		MismatchedChunkType(ChunkType expected, ChunkType given)
			: expected_chunk_type{expected}, given_chunk_type{given} {}

		[[nodiscard]] auto to_string() const -> std::string {
			return std::format("Expected chunk type {} but got {}.",
				chunk_type_to_string(expected_chunk_type),
				chunk_type_to_string(given_chunk_type));
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
		UnrecognizedChunkTypeString,
		MismatchedChunkType
	> variant;

	[[nodiscard]] auto to_code() const -> std::uint32_t {
		return std::visit([](const auto& err) -> std::uint32_t
			{ return err.code; }, variant);
	}

	[[nodiscard]] auto to_string() const -> std::string {
		return std::visit([](const auto& err) -> std::string
			{ return err.to_string(); }, variant);
	}

	static auto from(BinaryError err) -> XfbinError;
};

}

#endif
