#ifndef KOJO_NUCC_XFBIN
#define KOJO_NUCC_XFBIN

#include <nucc/game.hpp>
#include <nucc/page_new.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class XFBIN {
public:
    XFBIN() = default;
    XFBIN(const std::filesystem::path);
    XFBIN(kojo::binary_view, size_t);
    ~XFBIN() = default;

    void load(const std::filesystem::path);
    void load(kojo::binary_view, size_t);

    std::string filename() const { return m_filename; }
    Game game() const { return m_game; }
    constexpr std::string_view magic() const { return MAGIC; }
    constexpr std::uint32_t version() const { return VERSION; }

    ChunkType get_type(std::uint32_t map_index) const;
    std::string_view get_path(std::uint32_t map_index) const;
    std::string_view get_name(std::uint32_t map_index) const;

private:
    std::string m_filename;
    Game m_game{Game::UNKNOWN};
    static constexpr std::string_view MAGIC{"NUCC"}; // Required of an XFBIN to start with these 4 bytes.
    static const std::uint32_t VERSION{121}; // Expected of all relevant XFBINs.
    static const std::uint32_t HEADER_SIZE{12};

    size_t size;

    std::vector<std::string_view> types, paths, names;
    struct Chunk_Map {
        u32 type_index;
        u32 path_index; // Note that index 0 is usually empty ("") due to nuccChunkNull.
        u32 name_index; // Note that index 0 is usually empty ("") due to nuccChunkNull.
    };
    std::vector<Chunk_Map> maps;
    struct Extra_Indices {
        u32 name_index; // Used for clones of same clumps - optimisation feature.
        u32 map_index;
    };
    std::vector<Extra_Indices> extra_indices; // Used (mostly) for animations.
    std::vector<u32> map_indices;
    std::uint32_t running_map_offset{0};       /** Running total for page chunk map offsets. */
    std::uint32_t running_extra_offset{0};     /** Running total for page extra map offsets. */

    std::vector<Page> pages;

    void read(kojo::binary_view);
    void read_header(kojo::binary_view);
    void read_index(kojo::binary_view);
    void read_chunks(kojo::binary_view);
};

}

#endif // KOJO_NUCC_XFBIN