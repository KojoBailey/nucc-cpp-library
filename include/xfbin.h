#include <cstdint>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

enum class CC2_Game {
/*
    List of CyberConnect2 games that use XFBIN files.
    In order from old to new.
*/
    UNKNOWN = 0,
    NSUNS3, // Naruto Shippuden: Ultimate Ninja Storm 3
    ASB,    // JoJo's Bizarre Adventure: All-Star Battle
    NSUNSR, // Naruto Shippuden: Ultimate Ninja Storm Revolution
    EOH,    // JoJo's Bizarre Adventure: Eyes of Heaven
    NSUNS4, // Naruto Shippuden: Ultimate Ninja Storm 4
    ASBR,   // JoJo's Bizarre Adventure: All-Star Battle R
    NXBUNSC // NARUTO X BORUTO Ultimate Ninja STORM CONNECTIONS
};

class XFBIN {
public:
    std::string name; // Filename, not internally included.
    CC2_Game game;
    const char magic[4] = {'N', 'U', 'C', 'C'}; // If a file doesn't begin with these bytes, it's not recognised as an XFBIN.
    std::uint32_t version;
    std::vector<Chunk> chunks;

    std::uint32_t running_map_offset; // Running total for page chunk map offsets.
    std::uint32_t running_extra_offset; // Running total for page extra map offsets.

    void Load(std::filesystem::path path);
    XFBIN(std::filesystem::path path = "") {
        if (path != "") {
            Load(path);
        }
    }
};

class Chunk {
public:
    XFBIN* xfbin;
    enum Type;
    Type type;

    std::uint32_t size; // Size of entire chunk in bytes.
    std::uint32_t map_index;
    std::uint16_t version; // May differ from XFBIN version.
    std::uint16_t unk;
};

enum Chunk::Type {
    nuccChunkNull = 0,  // Always empty. Might be required for each page?
    nuccChunkIndex,     // Contains the XFBIN's metadata, including information about the file's chunks.
    nuccChunkPage,      // Signifies when one page of chunks ends, and what offset to use for later page chunk indices.
    nuccChunkAnm,
    nuccChunkBillboard,
    nuccChunkBinary,    // "Miscellaneous" binary data, including specific formats like XML.
    nuccChunkClump,
    nuccChunkCoord,
    nuccChunkDynamics,
    nuccChunkMaterial,
    nuccChunkModel,
    nuccChunkParticle,
    nuccChunkSprite,
    nuccChunkTexture,
    nuccChunkTrail
};

class nuccChunkIndex : Chunk {
/*
    Contains information about the XFBIN itself.
    Chunks refer to this's maps for their name, type, and source-file path.
*/
public:
    nuccChunkIndex() {
        type = Type::nuccChunkIndex;
    }

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
    
    std::vector<std::string> types; // nuccChunk[Type], e.g. `nuccChunkIndex`.
    std::vector<std::string> paths; // Likely the data source paths used to compile the XFBIN.
    std::vector<std::string> names;

    struct Chunk_Map;
    std::vector<Chunk_Map> maps;
    struct Extra_Indices;
    std::vector<Extra_Indices> extra_indices; // Used (mostly) for animations.
    std::vector<std::uint32_t> map_indices;

    std::string Get_Type(std::uint32_t map_index) {
        return types[maps[map_indices[map_index + xfbin->running_map_offset]].type_index];
    }
    std::string Get_Path(std::uint32_t map_index) {
        return types[maps[map_indices[map_index + xfbin->running_map_offset]].path_index];
    }
    std::string Get_Name(std::uint32_t map_index) {
        return types[maps[map_indices[map_index + xfbin->running_map_offset]].name_index];
    }
};

struct nuccChunkIndex::Chunk_Map {
    std::uint32_t type_index;
    std::uint32_t path_index; // Note that index 0 is usually empty ("") due to nuccChunkNull.
    std::uint32_t name_index; // Note that index 0 is usually empty ("") due to nuccChunkNull.
};
struct nuccChunkIndex::Extra_Indices {
    std::uint32_t name_index; // Used for clones of same clumps - optimisation feature.
    std::uint32_t map_index;
};

class nuccChunkPage : Chunk {
public:
    nuccChunkPage() {
        type = Type::nuccChunkPage;
    }

    std::uint32_t map_offset;
    std::uint32_t extra_offset;
};

class nuccChunkBinary : Chunk {
public:
    nuccChunkBinary() {
        type = Type::nuccChunkBinary;
    }

    /* Only one is of these two is to be used. */
    std::vector<char> binary_data;
    nlohmann::ordered_json json_data;
};