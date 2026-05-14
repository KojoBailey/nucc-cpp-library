#ifndef KOJO_XFBIN_HPP
#define KOJO_XFBIN_HPP

#include <xfbin/detail/error.hpp>

#include <kojo/binary.hpp>

#include <array>
#include <cstddef>
#include <expected>
#include <filesystem>
#include <span>
#include <string>
#include <vector>

namespace kojo {

class Xfbin {
public:
	static constexpr std::string_view FILE_SIGNATURE{"NUCC"}; // 4E 55 43 43 
        static constexpr std::uint32_t EXPECTED_VERSION{121};

	Xfbin() = default;
	~Xfbin() = default;

	[[nodiscard]] static auto from(const std::filesystem::path& path)
		-> std::expected<Xfbin, XfbinError>;
	[[nodiscard]] static auto from(std::span<const std::byte> span)
		-> std::expected<Xfbin, XfbinError>;
	[[nodiscard]] static auto from(const std::byte* ptr)
		-> std::expected<Xfbin, XfbinError>;

	[[nodiscard]] constexpr std::uint32_t get_version() const { return EXPECTED_VERSION; }

	// [[nodiscard]] auto get_chunks() const -> std::vector<Chunk>;
	// 
	// [[nodiscard]] const Chunk& fetch_chunk(
	// 	std::string_view name,
	// 	std::string_view path,
	// 	std::string_view type,
	// 	std::size_t index = 0
	// ) const;
	// [[nodiscard]] const Chunk& fetch_chunk_by_index(std::size_t index) const;
	// [[nodiscard]] const Chunk& fetch_chunk_by_name(std::string_view name, std::size_t index = 0) const;
	// [[nodiscard]] const Chunk& fetch_chunk_by_path(std::string_view path, std::size_t index = 0) const;
	// [[nodiscard]] const Chunk& fetch_chunk_by_type(std::string_view type, std::size_t index = 0) const;

	auto write_to_file(const std::filesystem::path& path) const -> std::expected<void, XfbinError>;
	auto write_to_vector() const -> std::expected<std::vector<std::byte>, XfbinError>;
	auto write_to_memory(std::byte* ptr) const -> std::expected<void, XfbinError>;

private:
        static constexpr std::uint32_t CHUNK_HEADER_SIZE{12};

	std::vector<std::string> types{};
	std::vector<std::string> paths{};
	std::vector<std::string> names{};

        struct chunk_map {
                std::uint32_t type_index;
                std::uint32_t path_index; // Index 0 is usually empty ("") due to nuccChunkNull.
                std::uint32_t name_index; // Index 0 is usually empty ("") due to nuccChunkNull.
        };
        std::vector<chunk_map> maps{};

        struct extra_indices {
                std::uint32_t name_index; // Used for clones of same clumps - optimisation feature.
                std::uint32_t map_index;
        };
        std::vector<extra_indices> extra_indices{}; // Used (mostly) for animations.

        std::vector<std::uint32_t> map_indices{};

	// struct Cryptor {
	// 	Cryptor() = default;
	//
	// 	bool should_decrypt{false};
	//
	// 	void load(const std::array<std::uint8_t, 8> key);
	//
 //       		void reset_state();
	//
	// 	// In-place safe if data_out == data_in.
	// 	void crypt(std::uint8_t* data_out, const std::size_t);
	//
	// 	std::array<std::uint8_t, 8> m_key{};
	//
	// 	std::uint32_t v1{};
	// 	std::uint32_t v2{};
	// 	std::uint32_t v3{};
	// 	std::uint32_t v4{};
	//
	// 	void roll_key(std::array<std::uint8_t, 4> xor_key);
	// };
	// Cryptor decryptor{};
};

}

#endif
