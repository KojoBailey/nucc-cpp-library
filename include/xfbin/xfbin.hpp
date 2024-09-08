#ifndef KOJO_NUCC_PLUS_PLUS
#define KOJO_NUCC_PLUS_PLUS

#include "external/binary-lib/binary/binary.hpp"
#include <unordered_map>
#include <unordered_set>

#include "chunks/chunks.hpp"

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

enum Optimise {
    MATCH = 0,  // Matches whatever chunks the XFBIN already has.
    TRUE,       // Removes unnecessary chunks and their metadata.
    FALSE       // Adds the unnecessary chunks where they're "supposed to be".
};

// Forward declaration.
class XFBIN;
struct Index;

/**
 * @brief Stores the general data of a chunk, but does not parse data into chunk-specific structures. 
 * @post Must be defined before the XFBIN class. Forward declaration doesn't solve this.
*/
struct OLD_Chunk {
public:
    XFBIN* xfbin;               /** @note May be unnecessary and therefore removed. */
    ChunkType type;             /** @warning Must match the struct used (e.g. `Binary` for nuccChunkBinary). */
    std::string path;           /** Internal chunk file path. @note Can usually be used to uniquely ID chunks. */
    std::string name;           /** Chunk name. @warning Typically the same across game versions, so not reliable for IDs. */

    std::uint32_t size{0};      /** Size of entire chunk in bytes. */
    std::uint32_t map_index;
    std::uint16_t version{121}; /** May differ from XFBIN version. */
    std::uint16_t unk;          /** @note Unknown, but may be animation-related. */
    
    binary data;

    Chunk() {};
    Chunk(ChunkType type_input, std::string path_input, std::string name_input) {
        type = type_input;
        path = path_input;
        name = name_input;
    }

    std::string type_string() {
        switch(type) {
            case ChunkType::Null        : return "nuccChunkNull";
            case ChunkType::Index       : return "nuccChunkIndex";
            case ChunkType::Page        : return "nuccChunkPage";
            case ChunkType::Anm         : return "nuccChunkAnm";
            case ChunkType::Billboard   : return "nuccChunkBillboard";
            case ChunkType::Binary      : return "nuccChunkBinary";
            case ChunkType::Camera      : return "nuccChunkCamera";
            case ChunkType::Clump       : return "nuccChunkClump";
            case ChunkType::Coord       : return "nuccChunkCoord";
            case ChunkType::Dynamic     : return "nuccChunkDynamic";
            case ChunkType::LightDirc   : return "nuccChunkLightDirc";
            case ChunkType::Material    : return "nuccChunkMaterial";
            case ChunkType::Model       : return "nuccChunkModel";
            case ChunkType::Nub         : return "nuccChunkNub";
            case ChunkType::Particle    : return "nuccChunkParticle";
            case ChunkType::Sprite      : return "nuccChunkSprite";
            case ChunkType::Texture     : return "nuccChunkTexture";
            case ChunkType::Trail       : return "nuccChunkTrail";
            default                     : return "nuccChunkUnknown";
        }
    }

    void update_data(std::vector<unsigned char> new_data) {
        data.load(new_data.data(), 0, new_data.size());
        size = data.size();
    }
    void update_data(unsigned char* new_data) {
        data.load(new_data);
    }

    unsigned char* get_data() {
        return data.data();
    }
};

/**
 * A class for storing XFBIN file information, a file format created by CyberConnect2 for use in their NUCC engine.
 */
class XFBIN {
public:
    // VARIABLES ↓↓↓

    std::string name;                           /** External filename, not included within the XFBIN file itself. */
    Game game;                                  /** Game that the XFBIN is from. */

    const char* magic = "NUCC";                 /** If a file doesn't begin with these bytes, it's not recognised as an XFBIN. */
    std::uint32_t version;                      /** e.g. `121` = 1.2.1 */
    std::vector<Chunk> chunks;                  /** @note First chunk is always nuccChunkIndex. */
    Index* index;

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
    Chunk* get_chunk(ChunkType type_input) {
        for (auto& chunk : chunks) {
            if (chunk.type == type_input)
                return &chunk;
        }
        return nullptr;
    }
    
    size_t add_chunk(ChunkType type_input = ChunkType::Binary, std::string path_input = "", std::string name_input = "") {
        // First make Index exist.
        if (chunks.size() == 0) {
            chunks.push_back({});
            chunks[0].type = nucc::ChunkType::Index;
        }

        chunks.push_back({});
        size_t new_chunk_index = chunks.size() - 1;
        chunks[new_chunk_index].xfbin = this;
        
        chunks[new_chunk_index].type = type_input;
        chunks[new_chunk_index].path = path_input;
        chunks[new_chunk_index].name = name_input;

        return new_chunk_index;
    }

    

    void load(std::string input_path) {
        file.load(input_path);
        read();
    }
    void load(std::vector<unsigned char>& vector_data, size_t start = 0, size_t end = -1) {
        file.load(vector_data, start, end);
        read();
    }
    void load(void* pointer_data) {
        file.load(pointer_data);
        read();
    }

    void write(std::filesystem::path output_path, Optimise optimise = Optimise::MATCH);

    // CON/DESTRUCTORS ↓↓↓

    XFBIN() {};
    XFBIN(std::filesystem::path input_path) {
        load(input_path);
    }
    XFBIN(std::vector<unsigned char>& vector_data, size_t start = 0, size_t end = -1) {
        load(vector_data, start, end);
    }

private:
    binary file;

    void compare_error(bool comparison, Error error) {
        if (comparison) throw std::runtime_error(get_error_str(error));
    }

    void read();
};

/**
 * Separates chunks into distinct groups.
 * @note Might be because of clumps.
*/
struct Page {
    Chunk* metadata;

    std::uint32_t map_offset;
    std::uint32_t extra_offset;

    Page(Chunk& chunk) {
        metadata = &chunk;
        metadata->data.set_pos(0);
        if (metadata->type != ChunkType::Page) 
            throw std::runtime_error("Cannot initialise nuccPage with non-nuccPage data.");

        map_offset = metadata->data.read<std::uint32_t>(std::endian::big);
        extra_offset = metadata->data.read<std::uint32_t>(std::endian::big);
    }

    void write_to_data(binary& data) {
        data.clear();

        data.write<std::uint32_t>(map_offset, std::endian::big);
        data.write<std::uint32_t>(extra_offset, std::endian::big);
    }
};

/**
 * Contains information about the XFBIN's chunks itself, which chunks (including itself) refer to.
*/
struct Index {
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

    std::string get_type(std::uint32_t map_index) {
        return types[maps[map_indices[map_index + metadata->xfbin->running_map_offset]].type_index];
    }
    std::string get_path(std::uint32_t map_index) {
        return paths[maps[map_indices[map_index + metadata->xfbin->running_map_offset]].path_index];
    }
    std::string get_name(std::uint32_t map_index) {
        return names[maps[map_indices[map_index + metadata->xfbin->running_map_offset]].name_index];
    }

    Index() {};
    Index(Chunk* chunk) {
        metadata = chunk;
        metadata->data.cursor = 0;
        if (metadata->type != ChunkType::Index) 
            throw std::runtime_error("Cannot initialise nuccIndex with non-nuccIndex data.");

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

    void calculate(std::vector<Chunk>& chunks, Optimise optimise) {
        std::unordered_map<std::string, std::uint32_t> type_tracker;
        std::unordered_map<std::string, std::uint32_t> path_tracker;
        std::unordered_map<std::string, std::uint32_t> name_tracker;
        std::unordered_map<std::string, std::uint32_t> map_tracker;
        types.clear();
        paths.clear();
        names.clear();
        maps.clear();
        type_size = 0;
        path_size = 0;
        name_size = 0;
        map_size = 0;

        for (auto& chunk : chunks) {
            if (!(optimise == Optimise::TRUE && chunk.type == ChunkType::Null)) {
                std::string type = chunk.type_string();
                if ( type_tracker.count(type) == 0 ) {
                    type_tracker[type] = type_tracker.size();
                    types.push_back(type);
                    type_size += type.size() + 1;
                }
                if ( path_tracker.count(chunk.path) == 0 ) {
                    path_tracker[chunk.path] = path_tracker.size();
                    paths.push_back(chunk.path);
                    path_size += chunk.path.size() + 1;
                }
                if ( name_tracker.count(chunk.name) == 0 ) {
                    name_tracker[chunk.name] = name_tracker.size();
                    names.push_back(chunk.name);
                    name_size += chunk.name.size() + 1;
                }

                Chunk_Map temp_map = {type_tracker[type], path_tracker[chunk.path], name_tracker[chunk.name]};
                std::string map_string = std::to_string(temp_map.type_index)
                    + "-" + std::to_string(temp_map.path_index)
                    + "-" + std::to_string(temp_map.name_index);
                if ( map_tracker.count(map_string) == 0 ) {
                    map_tracker[map_string] = map_tracker.size();
                    maps.push_back(temp_map);
                    map_size += sizeof(Chunk_Map);
                    map_indices.push_back(map_tracker[map_string]);
                }
                chunk.map_index = map_indices[map_tracker[map_string]];

                if (chunk.type == ChunkType::Page) {
                    Page page(chunk);
                    page.map_offset = 0;
                    page.write_to_data(chunk.data);
                }
            }
        }

        type_count = types.size();
        path_count = paths.size();
        name_count = names.size();
        map_count = maps.size();
        map_indices_count = map_indices.size();
    }

    void write_to_data(binary& data) {
        data.clear();

        data.write<std::uint32_t>(type_count, std::endian::big);
        data.write<std::uint32_t>(type_size, std::endian::big);
        data.write<std::uint32_t>(path_count, std::endian::big);
        data.write<std::uint32_t>(path_size, std::endian::big);
        data.write<std::uint32_t>(name_count, std::endian::big);
        data.write<std::uint32_t>(name_size, std::endian::big);
        data.write<std::uint32_t>(map_count, std::endian::big);
        data.write<std::uint32_t>(map_size, std::endian::big);
        data.write<std::uint32_t>(map_indices_count, std::endian::big);
        data.write<std::uint32_t>(extra_indices_count, std::endian::big);

        for (auto& type : types) data.write<std::string>(type);
        for (auto& path : paths) data.write<std::string>(path);
        for (auto& name : names) data.write<std::string>(name);

        data.align_by(4);

        for (auto& map : maps) {
            data.write<std::uint32_t>(map.type_index, std::endian::big);
            data.write<std::uint32_t>(map.path_index, std::endian::big);
            data.write<std::uint32_t>(map.name_index, std::endian::big);
        }
        for (auto& extra_indices_group : extra_indices) {
            data.write<std::uint32_t>(extra_indices_group.name_index, std::endian::big);
            data.write<std::uint32_t>(extra_indices_group.map_index, std::endian::big);
        }
        for (auto& map_index : map_indices)
            data.write<std::uint32_t>(map_index, std::endian::big);
    }
};

/**
 * Contains miscellaneous binary data, including formats not explicitly supported like XML.
*/
struct OLD_Binary {
    Chunk* metadata;

    /* Only one is of these two is to be used. */
    std::uint32_t size;

    binary* binary_data;

    Binary(Chunk* chunk) {
        metadata = chunk;
        metadata->data.cursor = 0;
        if (metadata->type != ChunkType::Binary) 
            throw std::runtime_error("Cannot initialise nuccBinary with non-nuccBinary data.");

        size = chunk->data.read<std::uint32_t>(std::endian::big);
        binary_data = &chunk->data;
    }
};

struct NUT_Texture {
    std::uint32_t total_size, data_size;
    std::uint16_t header_size;
    std::uint8_t minimap_count;
    std::uint8_t pixel_format;
    std::uint16_t width, height;
    const std::string magic = "GIDX";
    std::uint32_t hash_id;
    std::vector<char> data;
};

struct NUT {
    const std::string magic = "NTP3";
    std::uint16_t version;
    std::uint16_t count;
    std::vector<NUT_Texture> textures;
};

struct Texture {
    Chunk* metadata;

    std::uint16_t unk0;
    std::uint16_t width, height;
    std::uint16_t unk6;
    std::uint32_t size;
    NUT nut;

    Texture(Chunk* chunk) {
        metadata = chunk;
        metadata->data.cursor = 0;
        if (metadata->type != ChunkType::Texture) 
            throw std::runtime_error("Cannot initialise nuccTexture with non-nuccTexture data.");
    }
};

void XFBIN::read() {
    // Read header.
    std::string read_magic = file.read<std::string>(4);
    compare_error(magic != read_magic, Error::BAD_MAGIC);

    version = file.read<std::uint32_t>(std::endian::big);
    compare_error(version != 121, Error::UNSUPPORTED);

    file.read<std::uint64_t>(std::endian::big); /** Flags. @note Parse these at some point. */

    // Read chunks.
    for (int i = 0; file.cursor < file.size(); i++) {
        chunks.push_back({});
        chunks[i].xfbin = this;
        chunks[i].size = file.read<std::uint32_t>(std::endian::big);
        if (i == 0) { // If nuccIndex, chunk header size won't suffice (honestly, a design flaw).
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

    // Read nuccIndex.
    if (&chunks[0]) {
        chunks[0].type = ChunkType::Index;
        index = new Index(&chunks[0]);
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
            Page page(chunk);
            running_map_offset += page.map_offset;
            running_extra_offset += page.extra_offset;
        }
    }
}
void XFBIN::write(std::filesystem::path output_path, Optimise optimise) {
    // for (int i = 0; i < chunks.size(); i++) {
    //     index->maps.push_back({});
    //     index->map_indices.push_back(0);

    //     for (std::uint32_t& j = index->maps[i].name_index; j < index->names.size(); j++) {
    //         if (index->names[j] == chunks[i].name) {
    //             break;
    //         } else if (j + 1 == index->names.size()) {
    //             index->names.push_back(chunks[i].name);
    //             j++;
    //         }
    //     }
    // }

    binary output;

    output.write<std::string>("NUCC", 4);
    output.write<std::uint32_t>(version, std::endian::big);
    output.write<std::uint64_t>(0, std::endian::big);

    Index index;
    index.calculate(chunks, optimise);
    index.write_to_data(chunks[0].data);

    /* nuccChunk */
    for (auto& chunk : chunks) {
        if (!(optimise == Optimise::TRUE && chunk.type == ChunkType::Null)) {
            output.write<std::uint32_t>(chunk.size, std::endian::big); // Size
            output.write<std::uint32_t>(chunk.map_index, std::endian::big); // Map Index
            output.write<std::uint16_t>(chunk.version, std::endian::big); // Version
            output.write<std::uint16_t>(chunk.unk, std::endian::big); // unk
            output.write<std::vector<unsigned char>>(chunk.data.data); // Data
        }
    }

    output.dump_file(output_path);
}

} // namespace nucc

#endif // KOJO_NUCC_PLUS_PLUS