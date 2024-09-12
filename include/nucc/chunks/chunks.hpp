#ifndef KOJO_NUCC_CHUNKS
#define KOJO_NUCC_CHUNKS

#include "../external/binary-lib/binary/binary.hpp"

namespace nucc {

/**
 * List of NUCC chunk types used in XFBINs.
*/
enum class Chunk_Type {
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

// #include "chunks/anm.hpp"
// #include "binary.hpp"
// #include "chunks/clump.hpp"
// #include "chunks/index.hpp"
// #include "chunks/model.hpp"
// #include "chunks/nub.hpp"
// #include "chunks/page.hpp"
// #include "chunks/texture.hpp"

/**
 * @brief Stores the general data of a chunk, but does not parse data into chunk-specific structures. 
 * @post Must be defined before the XFBIN class. Forward declaration doesn't solve this.
*/
class Chunk {
public:
    Chunk_Type type{Chunk_Type::Null};  /** @warning Must match the struct used (e.g. `Binary` for nuccChunkBinary). */
    std::string path;                   /** Internal chunk file path. @note Can usually be used to uniquely ID chunks. */
    std::string name;                   /** Chunk name. @warning Typically the same across game versions, so not reliable for IDs. */

    std::uint32_t size{0};              /** Size of entire chunk in bytes. */
    std::uint32_t map_index{0};
    std::uint16_t version{121};         /** May differ from XFBIN version. */
    std::uint16_t unk{0};               /** @note Unknown, but may be animation-related. */

    kojo::binary storage;

    Chunk() = default;
    Chunk(Chunk* chunk) {
        load(chunk);
    }

    std::string type_as_string() {
        switch (type) {
            case Chunk_Type::Null        : return "nuccChunkNull";
            case Chunk_Type::Index       : return "nuccChunkIndex";
            case Chunk_Type::Page        : return "nuccChunkPage";
            case Chunk_Type::Anm         : return "nuccChunkAnm";
            case Chunk_Type::Billboard   : return "nuccChunkBillboard";
            case Chunk_Type::Binary      : return "nuccChunkBinary";
            case Chunk_Type::Camera      : return "nuccChunkCamera";
            case Chunk_Type::Clump       : return "nuccChunkClump";
            case Chunk_Type::Coord       : return "nuccChunkCoord";
            case Chunk_Type::Dynamic     : return "nuccChunkDynamic";
            case Chunk_Type::LightDirc   : return "nuccChunkLightDirc";
            case Chunk_Type::Material    : return "nuccChunkMaterial";
            case Chunk_Type::Model       : return "nuccChunkModel";
            case Chunk_Type::Nub         : return "nuccChunkNub";
            case Chunk_Type::Particle    : return "nuccChunkParticle";
            case Chunk_Type::Sprite      : return "nuccChunkSprite";
            case Chunk_Type::Texture     : return "nuccChunkTexture";
            case Chunk_Type::Trail       : return "nuccChunkTrail";
            default                     : return "nuccChunkUnknown";
        }
    }

    int load(void* input, size_t pos = 0) {
        if (input == nullptr) return -1;
        kojo::binary buffer{input, pos};
        size = buffer.read<std::uint32_t>(kojo::endian::big);
        map_index = buffer.read<std::uint32_t>(kojo::endian::big);
        version = buffer.read<std::uint16_t>(kojo::endian::big);
        unk = buffer.read<std::uint16_t>(kojo::endian::big);
        storage.load(buffer, buffer.get_pos(), size);
        parse();
        return 0;
    }
    int load(Chunk* chunk) {
        if (chunk == nullptr) return -1;
        type = chunk->type;
        name = chunk->name;
        path = chunk->path;
        size = chunk->size;
        map_index = chunk->map_index;
        version = chunk->version;
        unk = chunk->unk;
        storage.load(chunk->storage);
        parse();
        return 0;
    }

    virtual kojo::binary& dump() { return storage; }

protected:
    kojo::binary output;
    virtual void parse() {}
};

}

#endif // KOJO_NUCC_CHUNKS