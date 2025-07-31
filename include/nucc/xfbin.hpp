#ifndef KOJO_NUCC_XFBIN
#define KOJO_NUCC_XFBIN

#include <nucc/game.hpp>
#include <nucc/page.hpp>

#include <kojo/logger.hpp>
#include <kojo/binary.hpp>

namespace nucc {

class page;

class xfbin {
public:
    friend class page;
    friend class chunk;

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

    const std::vector<std::string>& types() const { return m_types; }
    const std::vector<std::string>& paths() const { return m_paths; }
    const std::vector<std::string>& names() const { return m_names; }

    const std::vector<page>& pages() const { return m_pages; }

    const page& get_page(size_t page_index = 0) const;
    const page& operator[](size_t page_index) const { return get_page(page_index); }

    const chunk& get_chunk(std::string_view chunk_name) const;
    const chunk& get_chunk(chunk_type chunk_type) const;
    const chunk& operator[](std::string_view chunk_name) const { return get_chunk(chunk_name); }
    const chunk& operator[](chunk_type chunk_type) const { return get_chunk(chunk_type); }

private:
    kojo::logger log{"NUCC++ Library"};

    static constexpr std::string_view MAGIC{"NUCC"}; // Required of an XFBIN to start with these 4 bytes.
    static constexpr std::uint32_t VERSION{121}; // Expected of all relevant XFBINs.
    static constexpr std::uint32_t HEADER_SIZE{12};

    size_t size;

    std::vector<std::string> m_types, m_paths, m_names;
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

    std::vector<nucc::page> m_pages;

    void read(kojo::binary_view);
    void read_header(kojo::binary_view&);
    void read_index(kojo::binary_view&);
    void read_chunks(kojo::binary_view&);
};

}

#endif // KOJO_NUCC_XFBIN