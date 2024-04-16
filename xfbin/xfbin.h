#ifndef KOJO_NUCC_PLUS_PLUS
#define KOJO_NUCC_PLUS_PLUS

#include "../binary/binary/binary.h"
#include <unordered_map>

/**
 * TO DO
 * - Writing
 * - More chunk support
*/

namespace kojo {
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

/**
 * List of NUCC chunk types used in XFBINs.
*/
enum class ChunkType {
    Null = 0,   // Unnecessary to support
    Index,      // Supported
    Page,       // Supported
    Anm,
    Billboard,
    Binary,     // WIP
    Camera,
    Clump,
    Coord,
    Dynamic,
    LightDirc,
    Material,
    Model,
    Nub,
    Particle,
    Sprite,
    Texture,    // WIP
    Trail
};

// Forward declaration.
class XFBIN;
struct ChunkIndex;

/**
 * @brief Stores the general data of a chunk, but does not parse data into chunk-specific structures. 
 * @post Must be defined before the XFBIN class. Forward declaration doesn't solve this.
*/
struct Chunk {
    XFBIN* xfbin;               /** @note May be unnecessary and therefore removed. */
    ChunkType type;         /** @warning Must match the struct used (e.g. `Binary` for `nuccChunkBinary`). */
    std::string path;           /** Internal chunk file path. @note Can usually be used to uniquely ID chunks. */
    std::string name;           /** Chunk name. @warning Typically the same across game versions, so not reliable for IDs. */

    std::uint32_t size;         /** Size of entire chunk in bytes. */
    std::uint32_t map_index;
    std::uint16_t version;      /** May differ from XFBIN version. */
    std::uint16_t unk;          /** @note Unknown, but may be animation-related. */
    
    binary data;
};

/**
 * A class for storing XFBIN file information, a file format created by CyberConnect2 for use in their NUCC engine.
 */
class XFBIN {
public:
    // VARIABLES ↓↓↓

    std::string name;                           /** External filename, not included within the XFBIN file itself. */
    Game game;                              /** Game that the XFBIN is from. */

    const std::string magic = "NUCC";           /** If a file doesn't begin with these bytes, it's not recognised as an XFBIN. */
    std::uint32_t version;                      /** e.g. `121` = 1.2.1 */
    std::vector<Chunk> chunks;              /** @note First chunk is always `nuccChunkIndex`. */
    ChunkIndex* index;

    std::uint32_t running_map_offset = 0;       /** Running total for page chunk map offsets. */
    std::uint32_t running_extra_offset = 0;     /** Running total for page extra map offsets. */

    /**
     * For API communication with programmers.
    */
    enum class Error {
        OK = 0,     /** No issues detected. */
        BAD_MAGIC,  /** File magic doesn't match `"NUCC"`. */
        UNSUPPORTED /** XFBIN version is not supported. @warning Only `121` is currently supported. More will be added as others are checked. */
    };

    // FUNCTIONS ↓↓↓

    std::string get_error_str(Error code) {
        switch (code) {
            case Error::OK : return "No issues detected."; break;
            case Error::BAD_MAGIC : return "File magic does not match `NUCC` or `4E 55 43 43`."; break;
            case Error::UNSUPPORTED : return "XFBIN version not supported. Must be `121`."; break;
            default: return "Invalid error code.";
        }
    }

    Chunk* get_chunk(size_t index) {
        return &chunks[index];
    }

    void load(std::filesystem::path input_path) {
        file.load(input_path);
        read();
    }
    void load(std::vector<char>& vector_data, size_t start = 0, size_t end = -1) {
        file.load(vector_data, start, end);
        read();
    }

    // CON/DESTRUCTORS ↓↓↓

    XFBIN() {};
    XFBIN(std::filesystem::path input_path) {
        load(input_path);
    }
    XFBIN(std::vector<char>& vector_data, size_t start = 0, size_t end = -1) {
        load(vector_data, start, end);
    }
    ~XFBIN();

private:
    binary file;

    void compare_error(bool comparison, Error error) {
        if (comparison) throw std::runtime_error(get_error_str(error));
    }

    void read();
};

/**
 * Contains information about the XFBIN's chunks itself, which chunks (including itself) refer to.
*/
struct ChunkIndex {
    Chunk* metadata;

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

    std::string get_type(std::uint32_t map_index) {
        return types[maps[map_indices[map_index + metadata->xfbin->running_map_offset]].type_index];
    }
    std::string get_path(std::uint32_t map_index) {
        return paths[maps[map_indices[map_index + metadata->xfbin->running_map_offset]].path_index];
    }
    std::string get_name(std::uint32_t map_index) {
        return names[maps[map_indices[map_index + metadata->xfbin->running_map_offset]].name_index];
    }

    ChunkIndex(Chunk* chunk) {
        metadata = chunk;
        metadata->data.cursor = 0;
        if (metadata->type != ChunkType::Index) 
            throw std::runtime_error("Cannot initialise nuccChunkIndex with non-nuccChunkIndex data.");

        // Quick way of filling out all consecutive uint32 variables.
        std::uint32_t* buffer = &type_count;
        for (int i = 0; i < 10; i++)
            buffer[i] = metadata->data.read<std::uint32_t>(std::endian::big);

        for (int i = 0; i < type_count; i++)
            types.push_back(metadata->data.read<std::string>());
        for (int i = 0; i < path_count; i++)
            paths.push_back(metadata->data.read<std::string>());
        for (int i = 0; i < name_count; i++)
            names.push_back(metadata->data.read<std::string>());

        metadata->data.align_by(4);

        for (int i = 0; i < map_count; i++)
            maps.push_back({
                metadata->data.read<std::uint32_t>(std::endian::big),
                metadata->data.read<std::uint32_t>(std::endian::big),
                metadata->data.read<std::uint32_t>(std::endian::big)
            });
        for (int i = 0; i < extra_indices_count; i++)
            extra_indices.push_back({
                metadata->data.read<std::uint32_t>(std::endian::big),
                metadata->data.read<std::uint32_t>(std::endian::big)
            });
        for (int i = 0; i < map_indices_count; i++)
            map_indices.push_back(metadata->data.read<std::uint32_t>(std::endian::big));
    }
};

/**
 * Separates chunks into distinct groups.
 * @note Might be because of clumps.
*/
struct ChunkPage {
    Chunk* metadata;

    std::uint32_t map_offset;
    std::uint32_t extra_offset;

    ChunkPage(Chunk& chunk) {
        metadata = &chunk;
        metadata->data.cursor = 0;
        if (metadata->type != ChunkType::Page) 
            throw std::runtime_error("Cannot initialise nuccChunkPage with non-nuccChunkPage data.");

        map_offset = metadata->data.read<std::uint32_t>(std::endian::big);
        extra_offset = metadata->data.read<std::uint32_t>(std::endian::big);
    }
};

/**
 * Contains miscellaneous binary data, including formats not explicitly supported like XML.
*/
struct ChunkBinary {
    Chunk* metadata;

    /* Only one is of these two is to be used. */
    std::uint32_t size;

    std::vector<char> binary_data;

    ChunkBinary(Chunk* chunk) {
        metadata = chunk;
        metadata->data.cursor = 0;
        if (metadata->type != ChunkType::Binary) 
            throw std::runtime_error("Cannot initialise nuccChunkBinary with non-nuccChunkBinary data.");
    }
};

struct Texture {
    std::uint32_t total_size, data_size;
    std::uint16_t header_size;
    std::uint8_t minimap_count;
    std::uint8_t pixel_format;
    std::uint16_t width, height;
    const std::string magic = "GIDX";
    std::uint32_t hash_id;
    std::vector<char> data;
};

struct NUTexture {
    const std::string magic = "NTP3";
    std::uint16_t version;
    std::uint16_t count;
    std::vector<Texture> textures;
};

struct ChunkTexture {
    Chunk* metadata;

    std::uint16_t unk0;
    std::uint16_t width, height;
    std::uint16_t unk6;
    std::uint32_t size;
    NUTexture nut;

    ChunkTexture(Chunk* chunk) {
        metadata = chunk;
        metadata->data.cursor = 0;
        if (metadata->type != ChunkType::Texture) 
            throw std::runtime_error("Cannot initialise nuccChunkBinary with non-nuccChunkBinary data.");
    }
};

void XFBIN::read() {
    // Read header.
    std::string read_magic = file.read<std::string>(4);
    compare_error(magic != read_magic, Error::BAD_MAGIC);

    version = file.read<std::uint32_t>(std::endian::big);
    compare_error(version != 121, Error::UNSUPPORTED);

    file.read<std::uint64_t>(std::endian::big); // Unknown purpose.

    // Read chunks.
    for (int i = 0; file.cursor < file.size(); i++) {
        chunks.push_back({});
        chunks[i].xfbin = this;
        chunks[i].size = file.read<std::uint32_t>(std::endian::big);
        if (i == 0) { // If nuccChunkIndex, chunk header size won't suffice (honestly, a design flaw).
            file.move(44);
            chunks[i].size += file.read<std::uint32_t>(std::endian::big) * 8;
            file.move(-48);
        }
        chunks[i].map_index = file.read<std::uint32_t>(std::endian::big);
        chunks[i].version = file.read<std::uint16_t>(std::endian::big);
        chunks[i].unk = file.read<std::uint16_t>(std::endian::big);
        chunks[i].data.load(file.data, file.cursor, file.cursor + chunks[i].size);
        file.move(chunks[i].size);
    }

    // Read nuccChunkIndex.
    if (&chunks[0]) {
        chunks[0].type = ChunkType::Index;
        index = new ChunkIndex(&chunks[0]);
    }

    // Fill out metadata of other chunks.
    std::unordered_map<std::string, ChunkType> str_to_ChunkType;
    str_to_ChunkType["nuccChunkNull"]       = ChunkType::Null;
    str_to_ChunkType["nuccChunkIndex"]      = ChunkType::Index;
    str_to_ChunkType["nuccChunkPage"]       = ChunkType::Page;
    str_to_ChunkType["nuccChunkAnm"]        = ChunkType::Anm;
    str_to_ChunkType["nuccChunkBillboard"]  = ChunkType::Billboard;
    str_to_ChunkType["nuccChunkBinary"]     = ChunkType::Binary;
    str_to_ChunkType["nuccChunkCamera"]     = ChunkType::Camera;
    str_to_ChunkType["nuccChunkClump"]      = ChunkType::Clump;
    str_to_ChunkType["nuccChunkCoord"]      = ChunkType::Coord;
    str_to_ChunkType["nuccChunkDynamic"]    = ChunkType::Dynamic;
    str_to_ChunkType["nuccChunkLightDirc"]  = ChunkType::LightDirc;
    str_to_ChunkType["nuccChunkMaterial"]   = ChunkType::Material;
    str_to_ChunkType["nuccChunkModel"]      = ChunkType::Model;
    str_to_ChunkType["nuccChunkNub"]        = ChunkType::Nub;
    str_to_ChunkType["nuccChunkParticle"]   = ChunkType::Particle;
    str_to_ChunkType["nuccChunkSprite"]     = ChunkType::Sprite;
    str_to_ChunkType["nuccChunkTexture"]    = ChunkType::Texture;
    str_to_ChunkType["nuccChunkTrail"]      = ChunkType::Trail;
    for (auto& chunk : chunks) {
        chunk.type = str_to_ChunkType[index->get_type(chunk.map_index)];
        chunk.path = index->get_path(chunk.map_index);
        chunk.name = index->get_name(chunk.map_index);
        if (chunk.type == ChunkType::Page) {
            ChunkPage page(chunk);
            running_map_offset += page.map_offset;
            running_extra_offset += page.extra_offset;
        }
    }
}

XFBIN::~XFBIN() {
    if (index != nullptr) delete[] index;
}

    } // namespace nucc
} // namespace kojo

#endif // KOJO_NUCC_PLUS_PLUS