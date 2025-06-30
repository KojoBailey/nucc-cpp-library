#include <nucc/chunk_types.hpp>

#include <string>
#include <unordered_map>

using namespace nucc;

std::string chunk_type_to_string(ChunkType chunk_type) {
    switch (chunk_type) {
        case ChunkType::Index                  : return "nuccChunkIndex";
        case ChunkType::Null                   : return "nuccChunkNull";
        case ChunkType::Page                   : return "nuccChunkPage";
        case ChunkType::Texture                : return "nuccChunkTexture";
        case ChunkType::Model                  : return "nuccChunkModel";
        case ChunkType::ModelHit               : return "nuccChunkModelHit";
        case ChunkType::Material               : return "nuccChunkMaterial";
        case ChunkType::Camera                 : return "nuccChunkCamera";
        case ChunkType::Anm                    : return "nuccChunkAnm";
        case ChunkType::Billboard              : return "nuccChunkBillboard";
        case ChunkType::Coord                  : return "nuccChunkCoord";
        case ChunkType::Sprite                 : return "nuccChunkSprite";
        case ChunkType::Particle               : return "nuccChunkParticle";
        case ChunkType::Dynamics               : return "nuccChunkDynamics";
        case ChunkType::Binary                 : return "nuccChunkBinary";
        case ChunkType::LightDirc              : return "nuccChunkLightDirc";
        case ChunkType::LightPoint             : return "nuccChunkLightPoint";
        case ChunkType::Ambient                : return "nuccChunkAmbient";
        case ChunkType::SpriteAnm              : return "nuccChunkSpriteAnm";
        case ChunkType::Font                   : return "nuccChunkFont";
        case ChunkType::Trail                  : return "nuccChunkTrail";
        case ChunkType::PrimitiveVertex        : return "nuccChunkPrimitiveVertex";
        case ChunkType::ModelPrimitiveBranch   : return "nuccChunkModelPrimitiveBranch";
        case ChunkType::MorphPrimitive         : return "nuccChunkMorphPrimitive";
        case ChunkType::LayerSet               : return "nuccChunkLayerSet";
        case ChunkType::ModelVertex            : return "nuccChunkModelVertex";
        case ChunkType::MorphModel             : return "nuccChunkMorphModel";
        case ChunkType::Clump                  : return "nuccChunkClump";
        case ChunkType::LightSet               : return "nuccChunkLightSet";
        case ChunkType::Sprite2                : return "nuccChunkSprite2";
        case ChunkType::Sprite2Anm             : return "nuccChunkSprite2Anm";
        case ChunkType::Nub                    : return "nuccChunkNub";
    }
    return "nuccChunkUnknown";
}

ChunkType string_to_chunk_type(std::string_view str) {
    static const std::unordered_map<std::string_view, ChunkType> string_chunk_type_map = {
        {"nuccChunkIndex", ChunkType::Index},
        {"nuccChunkNull", ChunkType::Null},
        {"nuccChunkUnknown", ChunkType::Unknown},
        {"nuccChunkPage", ChunkType::Page},
        {"nuccChunkTexture", ChunkType::Texture},
        {"nuccChunkModel", ChunkType::Model},
        {"nuccChunkModelHit", ChunkType::ModelHit},
        {"nuccChunkMaterial", ChunkType::Material},
        {"nuccChunkCamera", ChunkType::Camera},
        {"nuccChunkAnm", ChunkType::Anm},
        {"nuccChunkBillboard", ChunkType::Billboard},
        {"nuccChunkCoord", ChunkType::Coord},
        {"nuccChunkSprite", ChunkType::Sprite},
        {"nuccChunkParticle", ChunkType::Particle},
        {"nuccChunkDynamics", ChunkType::Dynamics},
        {"nuccChunkBinary", ChunkType::Binary}
    };

    auto iterator = string_chunk_type_map.find(str);
    if (iterator != string_chunk_type_map.end())
        return iterator->second;
    return ChunkType::Unknown;
}