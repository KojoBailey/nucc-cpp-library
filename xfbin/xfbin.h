#pragma once

#include "../binary/binary.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>

namespace kojo {

/**
 * List of CyberConnect2 games that use XFBIN files.
*/
enum class CC2_Game {
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
enum class nuccChunkType {
    Null = 0,
    Index,
    Page,
    Anm,
    Billboard,
    Binary,
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
    Texture,
    Trail
};

// Forward declaration.
class XFBIN;
struct nuccChunkIndex;

/**
 * @brief Stores the general data of a chunk, but does not parse data into chunk-specific structures. 
 * 
 * @post Must be defined before the XFBIN class. Forward declaration doesn't solve this.
*/
struct nuccChunk {
    XFBIN* xfbin;               /** @note May be unnecessary and therefore removed. */
    nuccChunkType type;         /** @warning Must match the struct used (e.g. `Binary` for `nuccChunkBinary`). */
    std::string path;           /** Internal chunk file path. @note Can usually be used to uniquely ID chunks. */
    std::string name;           /** Chunk name. @warning Typically the same across game versions, so not reliable for IDs. */

    std::uint32_t size;         /** Size of entire chunk in bytes. */
    std::uint32_t map_index;
    std::uint16_t version;      /** May differ from XFBIN version. */
    std::uint16_t unk;          /** @note Unknown, but may be animation-related. */
    Binary binary_data;

    nuccChunk() {
        
    }
};

/**
 * A class for storing XFBIN file information, a file format created by CyberConnect2 for use in their NUCC engine.
 */
class XFBIN {
public:
    // VARIABLES ↓↓↓

    std::string name;                           /** External filename, not included within the XFBIN file itself. */
    CC2_Game game;                              /** Game that the XFBIN is from. */

    const std::string magic = "NUCC";           /** If a file doesn't begin with these bytes, it's not recognised as an XFBIN. */
    std::uint32_t version;                      /** e.g. `121` = 1.2.1 */
    std::vector<nuccChunk> chunks;              /** @note First chunk is always `nuccChunkIndex`. */
    nuccChunkIndex* index;

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

    void load(std::filesystem::path input_path) {
        file.load(input_path);
        read();
    }

    nuccChunk* get_chunk(size_t index) {
        return &chunks[index];
    }

    // CONSTRUCTORS ↓↓↓

    XFBIN() {};
    XFBIN(const char* input_name) {
        name = (std::string)input_name;
    }

private:
    Binary file;

    void compare_error(bool comparison, Error error) {
        if (comparison) throw std::runtime_error(get_error_str(error));
    }

    void read();
};

/**
 * Contains information about the XFBIN's chunks itself, which chunks (including itself) refer to.
*/
struct nuccChunkIndex {
    nuccChunk* metadata;

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

    nuccChunkIndex(nuccChunk* chunk) {
        metadata = chunk;
        metadata->binary_data.set_pos(0);
        if (metadata->type != nuccChunkType::Index) 
            throw std::runtime_error("Cannot initialise nuccChunkIndex with non-nuccChunkIndex data.");

        // Quick way of filling out all consecutive uint32 variables.
        std::uint32_t* buffer = &type_count;
        for (int i = 0; i < 10; i++)
            buffer[i] = metadata->binary_data.b_read<std::uint32_t>(std::endian::big);

        for (int i = 0; i < type_count; i++)
            types.push_back(metadata->binary_data.b_read<std::string>());
        for (int i = 0; i < path_count; i++)
            paths.push_back(metadata->binary_data.b_read<std::string>());
        for (int i = 0; i < name_count; i++)
            names.push_back(metadata->binary_data.b_read<std::string>());

        metadata->binary_data.b_align(4);

        for (int i = 0; i < map_count; i++)
            maps.push_back({
                metadata->binary_data.b_read<std::uint32_t>(std::endian::big),
                metadata->binary_data.b_read<std::uint32_t>(std::endian::big),
                metadata->binary_data.b_read<std::uint32_t>(std::endian::big)
            });
        for (int i = 0; i < extra_indices_count; i++)
            extra_indices.push_back({
                metadata->binary_data.b_read<std::uint32_t>(std::endian::big),
                metadata->binary_data.b_read<std::uint32_t>(std::endian::big)
            });
        for (int i = 0; i < map_indices_count; i++)
            map_indices.push_back(metadata->binary_data.b_read<std::uint32_t>(std::endian::big));
    }
};

/**
 * Separates chunks into distinct groups.
 * @note Might be because of clumps.
*/
struct nuccChunkPage {
    nuccChunk* metadata;

    std::uint32_t map_offset;
    std::uint32_t extra_offset;

    nuccChunkPage(nuccChunk* chunk) {
        metadata = chunk;
        metadata->binary_data.set_pos(0);
        if (metadata->type != nuccChunkType::Page) 
            throw std::runtime_error("Cannot initialise nuccChunkPage with non-nuccChunkPage data.");

        map_offset = metadata->binary_data.b_read<std::uint32_t>(std::endian::big);
        extra_offset = metadata->binary_data.b_read<std::uint32_t>(std::endian::big);
    }
};

/**
 * Contains miscellaneous binary data, including formats not explicitly supported like XML.
*/
struct nuccChunkBinary {
    nuccChunk* metadata;

    /* Only one is of these two is to be used. */
    std::vector<char> binary_data;
    nlohmann::ordered_json json_data;

    nuccChunkBinary(nuccChunk* chunk) {
        metadata = chunk;
        metadata->binary_data.set_pos(0);
        if (metadata->type != nuccChunkType::Binary) 
            throw std::runtime_error("Cannot initialise nuccChunkBinary with non-nuccChunkBinary data.");
    }
};

void XFBIN::read() {
    // Read header.
    std::string read_magic = file.b_read<std::string>(4);
    compare_error(magic != read_magic, Error::BAD_MAGIC);

    version = file.b_read<std::uint32_t>(std::endian::big);
    compare_error(version != 121, Error::UNSUPPORTED);

    file.b_read<std::uint64_t>(std::endian::big); // Unknown purpose.

    // Read chunks.
    for (int i = 0; file.get_pos() < file.data.size(); i++) {
        chunks.push_back({});
        chunks[i].xfbin = this;
        chunks[i].size = file.b_read<std::uint32_t>(std::endian::big);
        if (i == 0) { // If nuccChunkIndex, chunk header size won't suffice (honestly, a design flaw).
            file.b_move(44);
            chunks[i].size += file.b_read<std::uint32_t>(std::endian::big) * 8;
            file.b_move(-48);
        }
        chunks[i].map_index = file.b_read<std::uint32_t>(std::endian::big);
        chunks[i].version = file.b_read<std::uint16_t>(std::endian::big);
        chunks[i].unk = file.b_read<std::uint16_t>(std::endian::big);
        chunks[i].binary_data.load(file.data, file.get_pos(), file.get_pos() + chunks[i].size);
        file.b_move(chunks[i].size);
    }

    // Read nuccChunkIndex.
    if (&chunks[0]) {
        chunks[0].type = nuccChunkType::Index;
        index = new nuccChunkIndex(&chunks[0]);
    }

    // Fill out metadata of other chunks.
    std::unordered_map<std::string, nuccChunkType> str_to_nuccChunkType;
    str_to_nuccChunkType["nuccChunkNull"]       = nuccChunkType::Null;
    str_to_nuccChunkType["nuccChunkIndex"]      = nuccChunkType::Index;
    str_to_nuccChunkType["nuccChunkPage"]       = nuccChunkType::Page;
    str_to_nuccChunkType["nuccChunkAnm"]        = nuccChunkType::Anm;
    str_to_nuccChunkType["nuccChunkBillboard"]  = nuccChunkType::Billboard;
    str_to_nuccChunkType["nuccChunkBinary"]     = nuccChunkType::Binary;
    str_to_nuccChunkType["nuccChunkCamera"]     = nuccChunkType::Camera;
    str_to_nuccChunkType["nuccChunkClump"]      = nuccChunkType::Clump;
    str_to_nuccChunkType["nuccChunkCoord"]      = nuccChunkType::Coord;
    str_to_nuccChunkType["nuccChunkDynamic"]    = nuccChunkType::Dynamic;
    str_to_nuccChunkType["nuccChunkLightDirc"]  = nuccChunkType::LightDirc;
    str_to_nuccChunkType["nuccChunkMaterial"]   = nuccChunkType::Material;
    str_to_nuccChunkType["nuccChunkModel"]      = nuccChunkType::Model;
    str_to_nuccChunkType["nuccChunkNub"]        = nuccChunkType::Nub;
    str_to_nuccChunkType["nuccChunkParticle"]   = nuccChunkType::Particle;
    str_to_nuccChunkType["nuccChunkSprite"]     = nuccChunkType::Sprite;
    str_to_nuccChunkType["nuccChunkTexture"]    = nuccChunkType::Texture;
    str_to_nuccChunkType["nuccChunkTrail"]      = nuccChunkType::Trail;
    for (auto& chunk : chunks) {
        chunk.type = str_to_nuccChunkType[index->get_type(chunk.map_index)];
        chunk.path = index->get_path(chunk.map_index);
        chunk.name = index->get_name(chunk.map_index);
        if (chunk.type == nuccChunkType::Page) {
            nuccChunkPage page(&chunk);
            running_map_offset += page.map_offset;
            running_extra_offset += page.extra_offset;
        }
    }
}

}