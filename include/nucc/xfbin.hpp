#ifndef KOJO_NUCC_PLUS_PLUS
#define KOJO_NUCC_PLUS_PLUS

#include <kojo/binary.hpp>

#include "error_handling.hpp"
#include "page.hpp"
#include "chunks/binary.hpp"

#include <filesystem>
#include <iostream>
#include <string_view>

namespace nucc {

/**
 * List of CyberConnect2 games that use XFBIN files.
*/
enum class Game {
    // In order from old to new.
    UNKNOWN = 0,
    NSUNS3, /** Naruto Shippuden: Ultimate Ninja Storm 3 */
    ASB,    /** JoJo's Bizarre Adventure: All-Star Battle */
    NSUNSR, /** Naruto Shippuden: Ultimate Ninja Storm Revolution */
    EOHPS3, /** JoJo's Bizarre Adventure: Eyes of Heaven (PlayStation 3) */
    EOHPS4, /** JoJo's Bizarre Adventure: Eyes of Heaven (PlayStation 4) */
    NSUNS4, /** Naruto Shippuden: Ultimate Ninja Storm 4 */
    ASBR,   /** JoJo's Bizarre Adventure: All-Star Battle R */
    NXBUNSC /** NARUTO X BORUTO Ultimate Ninja STORM CONNECTIONS */
};
std::string game_to_string(Game game);
Game string_to_game(std::string str);

enum class Optimize {
    MATCH = 0,  // Matches whatever chunks the XFBIN already has.
    YES,        // Removes unnecessary chunks and their metadata.
    NO          // Adds the unnecessary chunks where they're "supposed to be".
};

/**
 * A class for storing XFBIN file information, a file format created by CyberConnect2 for use in their NUCC engine.
 */
class XFBIN {
public:
    std::string name;                           /** External filename, not included within the XFBIN file itself. */
    Game game{Game::UNKNOWN};                   /** Game that the XFBIN is from. */
    std::string game_as_string();

    static constexpr std::string_view magic{"NUCC"};    /** If a file doesn't begin with these bytes, it's not recognised as an XFBIN. */
    std::uint32_t version{121};                         /** e.g. `121` = 1.2.1 */
    
    std::vector<Page> pages;

    /** Contains information about the XFBIN's chunks itself, which chunks (including itself) refer to. */
    struct Index {
        std::uint32_t type_count;
        std::uint32_t type_size;
        std::uint32_t path_count;
        std::uint32_t path_size;
        std::uint32_t name_count;
        std::uint32_t name_size;
        std::uint32_t map_count;
        std::uint32_t map_size;
        std::uint32_t map_indices_count;
        std::uint32_t extra_indices_count;
        
        std::vector<std::string> types; // nuccChunk[Type], e.g. "nuccChunkIndex".
        std::vector<std::string> paths; // Likely the data source paths used to compile the XFBIN.
        std::vector<std::string> names;

        struct Chunk_Map {
            std::uint32_t type_index;
            std::uint32_t path_index; // Note that index 0 is usually empty ("") due to nuccChunkNull.
            std::uint32_t name_index; // Note that index 0 is usually empty ("") due to nuccChunkNull.
        };
        std::vector<Chunk_Map> maps;
        struct Extra_Indices {
            std::uint32_t name_index; // Used for clones of same clumps - optimisation feature.
            std::uint32_t map_index;
        };
        std::vector<Extra_Indices> extra_indices; // Used (mostly) for animations.
        std::vector<std::uint32_t> map_indices;

        std::uint32_t running_map_offset = 0;       /** Running total for page chunk map offsets. */
        std::uint32_t running_extra_offset = 0;     /** Running total for page extra map offsets. */

        Chunk_Type  get_type(std::uint32_t map_index);
        std::string get_path(std::uint32_t map_index);
        std::string get_name(std::uint32_t map_index);
    } index;

    XFBIN();
    XFBIN(std::filesystem::path input_path);
    XFBIN(kojo::binary& input_data, size_t start = 0, size_t end = -1);
    XFBIN(void* pointer_data, size_t start = 0, size_t end = -1);
    ~XFBIN() = default;

    int load(std::filesystem::path input_path);
    int load(kojo::binary& input_data, size_t start = 0, size_t end = -1);
    int load(void* pointer_data, size_t start = 0, size_t end = -1);
    
    Chunk* fetch(Chunk_Type chunk_type, size_t index = 0);
    template<typename T = Chunk> T* fetch(std::string chunk_name, size_t index = 0) {
        size_t i = 0;
        for (auto& page : pages) {
            for (auto& chunk : page.chunks) {
                if (chunk.name == chunk_name || chunk.path == chunk_name) {
                    if (i == index) {
                        return (T*)&chunk;
                    } else {
                        i++;
                    }
                }
            }
        }
        return nullptr;
    }

    void write(std::string output_path, Optimize optimize = Optimize::MATCH);

private:
    kojo::binary input;
    kojo::binary output;

    int read();

    void calculate(Optimize optimize);
};

} // namespace nucc

#endif // KOJO_NUCC_PLUS_PLUS