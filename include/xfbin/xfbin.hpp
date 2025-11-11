#ifndef KOJO_XFBIN_HPP
#define KOJO_XFBIN_HPP

#include <kojo/binary.hpp>

#include <expected>
#include <filesystem>
#include <string>

namespace kojo {

class xfbin {
public:
	xfbin() = default;
	~xfbin() = default;

	std::string filename;

	/* Error handling */

	enum class error {
		ok,
		null_file,
		file_signature,
		version,
	};

	[[nodiscard]] inline static constexpr std::string_view error_string(error code)
	{
		switch (code) {
		case error::ok:
			return "No errors detected.";
		case error::null_file:
			return "File does not exist.";
		}

		return "Unknown error code.";
	}

	/* Loading */

	[[nodiscard]] static auto load(const std::filesystem::path& path) -> std::expected<xfbin, error>;

private:
	static constexpr std::string_view MAGIC{"NUCC"};    // Required of an XFBIN to start with these 4 bytes.
        static constexpr std::uint32_t VERSION{121};        // Expected of all relevant XFBINs.
        static constexpr std::uint32_t HEADER_SIZE{12};

	size_t m_size{0};

	/* Reading */
	auto read(kojo::binary_view) -> std::expected<void, error>;
	auto read_header(kojo::binary_view&) -> std::expected<void, error>;
	auto read_index(kojo::binary_view&) -> std::expected<void, error>;
	auto read_chunks(kojo::binary_view&) -> std::expected<void, error>;
};

}

#endif