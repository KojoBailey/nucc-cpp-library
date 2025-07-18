#ifndef KOJO_NUCC_XFBIN
#define KOJO_NUCC_XFBIN

#include <nucc/game.hpp>
#include <nucc/chunk_type.hpp>
#include <nucc/page_new.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class page;

class xfbin {
public:
    xfbin() = default;
    xfbin(const std::filesystem::path);
    xfbin(kojo::binary_view, size_t);
    ~xfbin() = default;

    void load(const std::filesystem::path);
    void load(kojo::binary_view, size_t);

    std::string filename;
    game game{game::unknown};
    constexpr std::string_view magic() const { return MAGIC; }
    constexpr std::uint32_t version() const { return VERSION; }

    chunk_type get_type(std::uint32_t map_index) const;
    std::string_view get_path(std::uint32_t map_index) const;
    std::string_view get_name(std::uint32_t map_index) const;

    const std::vector<std::string_view>& types() const { return m_types; }
    const std::vector<std::string_view>& paths() const { return m_paths; }
    const std::vector<std::string_view>& names() const { return m_names; }

    const std::vector<page>& pages() const { return m_pages; }

private:
    static constexpr std::string_view MAGIC{"NUCC"}; // Required of an XFBIN to start with these 4 bytes.
    static const std::uint32_t VERSION{121}; // Expected of all relevant XFBINs.
    static const std::uint32_t HEADER_SIZE{12};

    size_t size;

    std::vector<std::string_view> m_types, m_paths, m_names;
    struct chunk_map {
        std::uint32_t type_index;
        std::uint32_t path_index; // Note that index 0 is usually empty ("") due to nuccChunkNull.
        std::uint32_t name_index; // Note that index 0 is usually empty ("") due to nuccChunkNull.
    };
    std::vector<chunk_map> maps;
    struct extra_indices {
        std::uint32_t name_index; // Used for clones of same clumps - optimisation feature.
        std::uint32_t map_index;
    };
    std::vector<extra_indices> extra_indices; // Used (mostly) for animations.
    std::vector<std::uint32_t> map_indices;
    std::uint32_t running_map_offset{0};       /** Running total for page chunk map offsets. */
    std::uint32_t running_extra_offset{0};     /** Running total for page extra map offsets. */

    std::vector<page> m_pages;

    void read(kojo::binary_view);
    void read_header(kojo::binary_view);
    void read_index(kojo::binary_view);
    void read_chunks(kojo::binary_view);
};

}

#endif // KOJO_NUCC_XFBIN