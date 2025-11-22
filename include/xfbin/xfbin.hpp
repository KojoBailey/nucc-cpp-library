#ifndef KOJO_XFBIN_HPP
#define KOJO_XFBIN_HPP

#include <kojo/binary.hpp>

#include <array>
#include <expected>
#include <filesystem>
#include <string>

namespace kojo {

class Xfbin {
public:
	Xfbin() = default;
	~Xfbin() = default;

	std::string filename;

	/* Error-handling */

	enum class Error {
		ok,
		null_file,
		file_signature,
		version,
		cut_short,
	};

	[[nodiscard]] inline static constexpr std::string_view error_string(Error code)
	{
		switch (code) {
		case Error::ok:
			return "No errors detected.";
		case Error::null_file:
			return "File does not exist.";
		case Error::file_signature:
			return "Invalid file signature. Should match \"NUCC\" (4E 55 43 43).";
		case Error::version:
			return "Unknown file version. Expected 121.";
		case Error::cut_short:
			return "File data (size) shorter than expected.";
		}

		return "Unknown error code.";
	}

	/* Loading */

	[[nodiscard]] static auto load(const std::filesystem::path& path) -> std::expected<Xfbin, Error>;

private:
	static constexpr std::string_view FILE_SIGNATURE{"NUCC"};
        static constexpr std::uint32_t EXPECTED_VERSION{121};
        static constexpr std::uint32_t CHUNK_HEADER_SIZE{12};

	size_t m_size{0};

	std::vector<std::string> m_types{};
	std::vector<std::string> m_paths{};
	std::vector<std::string> m_names{};

        struct chunk_map {
                std::uint32_t type_index;
                std::uint32_t path_index; // Index 0 is usually empty ("") due to nuccChunkNull.
                std::uint32_t name_index; // Index 0 is usually empty ("") due to nuccChunkNull.
        };
        std::vector<chunk_map> m_maps{};

        struct extra_indices {
                std::uint32_t name_index; // Used for clones of same clumps - optimisation feature.
                std::uint32_t map_index;
        };
        std::vector<extra_indices> m_extra_indices{}; // Used (mostly) for animations.

        std::vector<std::uint32_t> m_map_indices{};

	class Cryptor {
	public:
		Cryptor() = default;

		bool should_decrypt{false};

		void load(const std::array<std::uint8_t, 8> key);

       		void reset_state();

		// In-place safe if data_out == data_in.
		void crypt(uint8_t* data_out, const size_t);

	private:
		std::array<std::uint8_t, 8> m_key{};

		std::uint32_t v1{};
		std::uint32_t v2{};
		std::uint32_t v3{};
		std::uint32_t v4{};

		void roll_key(std::array<std::uint8_t, 4> xor_key);
	};
	Cryptor m_decryptor{};

	auto read(kojo::binary_view) -> std::expected<void, Error>;
	auto read_header(kojo::binary_view&) -> std::expected<void, Error>;
	auto read_index(kojo::binary_view&) -> std::expected<void, Error>;
	auto read_chunks(kojo::binary_view&) -> std::expected<void, Error>;
};

}

#endif