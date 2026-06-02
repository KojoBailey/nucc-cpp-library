#ifndef KOJO_XFBIN_HPP
#define KOJO_XFBIN_HPP

#include <xfbin/page.hpp>
#include <xfbin/detail/error.hpp>

#include <kojo/binary.hpp>

#include <array>
#include <cstddef>
#include <expected>
#include <filesystem>
#include <span>
#include <string>
#include <vector>

namespace kojo::nucc {

class Xfbin {
	friend class XfbinReader;
public:
	static constexpr std::string_view FILE_SIGNATURE{"NUCC"}; // 4E 55 43 43 
        static constexpr std::uint32_t EXPECTED_VERSION{121};

	Xfbin() = default;
	~Xfbin() = default;

	[[nodiscard]] static auto from(const std::filesystem::path& path,
			std::array<std::uint8_t, 8> crypt_key = {})
		-> std::expected<Xfbin, XfbinError>;
	[[nodiscard]] static auto from(std::span<const std::byte> span,
			std::array<std::uint8_t, 8> crypt_key = {})
		-> std::expected<Xfbin, XfbinError>;
	[[nodiscard]] static auto from(const std::byte* ptr,
			std::array<std::uint8_t, 8> crypt_key = {})
		-> std::expected<Xfbin, XfbinError>;

	[[nodiscard]] static constexpr auto get_version()
		-> std::uint32_t { return EXPECTED_VERSION; }

	// [[nodiscard]] const Chunk& fetch_chunk(
	// 	std::string_view name,
	// 	std::string_view path,
	// 	std::string_view type,
	// 	std::size_t index = 0
	// ) const;
	// [[nodiscard]] const Chunk& fetch_chunk_by_name(std::string_view name, std::size_t index = 0) const;
	// [[nodiscard]] const Chunk& fetch_chunk_by_path(std::string_view path, std::size_t index = 0) const;
	// [[nodiscard]] const Chunk& fetch_chunk_by_type(std::string_view type, std::size_t index = 0) const;

	// [[nodiscard]] auto write_to_file(const std::filesystem::path& path) const
	// 	-> std::expected<void, XfbinError>;
	// [[nodiscard]] auto write_to_vector() const
	// 	-> std::expected<std::vector<std::byte>, XfbinError>;
	// [[nodiscard]] auto write_to_memory(std::byte* ptr) const
	// 	-> std::expected<void, XfbinError>;

	[[nodiscard]] auto get_types() const
		-> std::span<const ChunkType> { return types; }
	[[nodiscard]] auto get_paths() const
		-> std::span<const std::string> { return paths; }
	[[nodiscard]] auto get_names() const
		-> std::span<const std::string> { return names; }

	[[nodiscard]] auto get_pages() const
		-> std::span<const Page> { return pages; }

	[[nodiscard]] auto fetch_type_from_map_index(std::uint32_t map_index) const noexcept
		-> std::expected<ChunkType, XfbinError>;
	[[nodiscard]] auto fetch_path_from_map_index(std::uint32_t map_index) const noexcept
		-> std::expected<std::string_view, XfbinError>;
	[[nodiscard]] auto fetch_name_from_map_index(std::uint32_t map_index) const noexcept
		-> std::expected<std::string_view, XfbinError>;

	[[nodiscard]] auto fetch_chunk(
		std::optional<ChunkType> type,
		std::optional<std::string_view> path,
		std::optional<std::string_view> name,
		std::size_t index = 0
	) -> Chunk*;

	[[nodiscard]] auto fetch_chunk(
		std::optional<ChunkType> type,
		std::optional<std::string_view> path,
		std::optional<std::string_view> name,
		std::size_t index = 0
	) const -> const Chunk*;

	auto add_page() -> Page&;
	auto add_page(std::uint32_t chunk_map_offset, std::uint32_t extra_map_offset) -> Page&;

private:
	std::vector<ChunkType> types;
	std::vector<std::string> paths;
	std::vector<std::string> names;

        struct ChunkMap {
                std::uint32_t type_index;
                std::uint32_t path_index; // Index 0 is usually empty ("") due to nuccChunkNull.
                std::uint32_t name_index; // Index 0 is usually empty ("") due to nuccChunkNull.
        };
        std::vector<ChunkMap> maps;

        struct ExtraIndices {
                std::uint32_t name_index; // Used for clones of same clumps - optimisation feature.
                std::uint32_t map_index;
        };
        std::vector<ExtraIndices> extra_indices; // Used (mostly) for animations.

        std::vector<std::uint32_t> map_indices;

	std::vector<Page> pages;
};

}

#endif
