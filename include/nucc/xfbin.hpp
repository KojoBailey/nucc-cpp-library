#ifndef KOJO_NUCC_PLUS_PLUS
#define KOJO_NUCC_PLUS_PLUS

#include "external/binary-lib/binary/binary.hpp"

#include "page.hpp"
#include "chunks/binary.hpp"

#include <filesystem>
#include <iostream>

// #include <format>

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
    EOH,    /** JoJo's Bizarre Adventure: Eyes of Heaven */
    NSUNS4, /** Naruto Shippuden: Ultimate Ninja Storm 4 */
    ASBR,   /** JoJo's Bizarre Adventure: All-Star Battle R */
    NXBUNSC /** NARUTO X BORUTO Ultimate Ninja STORM CONNECTIONS */
};

enum Optimize {
    MATCH = 0,  // Matches whatever chunks the XFBIN already has.
    TRUE,       // Removes unnecessary chunks and their metadata.
    FALSE       // Adds the unnecessary chunks where they're "supposed to be".
};

/**
 * A class for storing XFBIN file information, a file format created by CyberConnect2 for use in their NUCC engine.
 */
class XFBIN {
public:
    std::string name;                           /** External filename, not included within the XFBIN file itself. */
    Game game{Game::UNKNOWN};                   /** Game that the XFBIN is from. */

    std::string magic{"NUCC"};                  /** If a file doesn't begin with these bytes, it's not recognised as an XFBIN. */
    std::uint32_t version{121};                 /** e.g. `121` = 1.2.1 */
    
    std::vector<Page> pages;

    XFBIN();
    XFBIN(std::filesystem::path input_path);
    XFBIN(kojo::binary& input_data, size_t start = 0, size_t end = -1);
    XFBIN(void* pointer_data, size_t start = 0, size_t end = -1);
    ~XFBIN() = default;
    
    /**
     * For API communication with programmers.
    */
    enum class Error_Code {
        OK = 0,         /** No issues detected. */
        INVALID_FILE,   /** File could not be loaded. */
        NULL_INPUT,     /** Input data is null. */
        MAGIC,          /** File magic does not match `NUCC` or `4E 55 43 43`. */
        VERSION         /** XFBIN version is not supported. @warning Only `121` is currently supported. More will be added as others are checked. */
    };

    struct Status {
        Error_Code code;
        int number;
        std::string message;
    };

    Status get_status();

    void load(std::filesystem::path input_path);
    void load(kojo::binary& input_data, size_t start = 0, size_t end = -1);
    void load(void* pointer_data, size_t start = 0, size_t end = -1);
    
    Chunk* fetch(std::string chunk_name, size_t index = 0);
    Chunk* fetch(Chunk_Type chunk_type, size_t index = 0);

    void write(std::string output_path, Optimize optimize = Optimize::MATCH);

private:
    kojo::binary input;
    kojo::binary output;

    /**
     * Contains information about the XFBIN's chunks itself, which chunks (including itself) refer to.
    */
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

    Error_Code status;

    void read();

    void calculate(Optimize optimize);

    void error();
};

} // namespace nucc

#endif // KOJO_NUCC_PLUS_PLUS