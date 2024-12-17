#ifndef KOJO_NUCC_CHUNKS
#define KOJO_NUCC_CHUNKS

#include <kojo/binary.hpp>

namespace nucc {

/**
 * List of NUCC chunk types used in XFBINs.
*/
enum class Chunk_Type {
    Index = 0,  // Supported
    Null,       // Supported
    Unknown,
    Page,       // Supported
    Texture,    // WIP
    Model,
    ModelHit,
    Material,
    Camera,
    Anm,
    Billboard,
    Coord,
    Sprite,
    Particle,
    Dynamics,
    Binary,     // WIP
    LightDirc,
    LightPoint,
    Ambient,
    SpriteAnm,
    Font,
    Trail,
    PrimitiveVertex,
    ModelPrimitiveBranch,
    MorphPrimitive,
    LayerSet,
    ModelVertex,
    MorphModel,
    Clump,
    LightSet,
    Sprite2,
    Sprite2Anm,
    Nub,
};

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
            case Chunk_Type::Index                  : return "nuccChunkIndex";
            case Chunk_Type::Null                   : return "nuccChunkNull";
            case Chunk_Type::Page                   : return "nuccChunkPage";
            case Chunk_Type::Texture                : return "nuccChunkTexture";
            case Chunk_Type::Model                  : return "nuccChunkModel";
            case Chunk_Type::ModelHit               : return "nuccChunkModelHit";
            case Chunk_Type::Material               : return "nuccChunkMaterial";
            case Chunk_Type::Camera                 : return "nuccChunkCamera";
            case Chunk_Type::Anm                    : return "nuccChunkAnm";
            case Chunk_Type::Billboard              : return "nuccChunkBillboard";
            case Chunk_Type::Coord                  : return "nuccChunkCoord";
            case Chunk_Type::Sprite                 : return "nuccChunkSprite";
            case Chunk_Type::Particle               : return "nuccChunkParticle";
            case Chunk_Type::Dynamics               : return "nuccChunkDynamics";
            case Chunk_Type::Binary                 : return "nuccChunkBinary";
            case Chunk_Type::LightDirc              : return "nuccChunkLightDirc";
            case Chunk_Type::LightPoint             : return "nuccChunkLightPoint";
            case Chunk_Type::Ambient                : return "nuccChunkAmbient";
            case Chunk_Type::SpriteAnm              : return "nuccChunkSpriteAnm";
            case Chunk_Type::Font                   : return "nuccChunkFont";
            case Chunk_Type::Trail                  : return "nuccChunkTrail";
            case Chunk_Type::PrimitiveVertex        : return "nuccChunkPrimitiveVertex";
            case Chunk_Type::ModelPrimitiveBranch   : return "nuccChunkModelPrimitiveBranch";
            case Chunk_Type::MorphPrimitive         : return "nuccChunkMorphPrimitive";
            case Chunk_Type::LayerSet               : return "nuccChunkLayerSet";
            case Chunk_Type::ModelVertex            : return "nuccChunkModelVertex";
            case Chunk_Type::MorphModel             : return "nuccChunkMorphModel";
            case Chunk_Type::Clump                  : return "nuccChunkClump";
            case Chunk_Type::LightSet               : return "nuccChunkLightSet";
            case Chunk_Type::Sprite2                : return "nuccChunkSprite2";
            case Chunk_Type::Sprite2Anm             : return "nuccChunkSprite2Anm";
            case Chunk_Type::Nub                    : return "nuccChunkNub";
            default                                 : return "nuccChunkUnknown";
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