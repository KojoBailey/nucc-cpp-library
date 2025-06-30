#ifndef KOJO_NUCC_CHUNK_TYPES
#define KOJO_NUCC_CHUNK_TYPES

#include <string>

namespace nucc {

enum class ChunkType {
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

std::string chunk_type_to_string(ChunkType);
ChunkType string_to_chunk_type(std::string_view);

}

#endif // KOJO_NUCC_CHUNK_TYPES