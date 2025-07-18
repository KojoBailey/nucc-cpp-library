#include <nucc/chunk_type.hpp>

#include <string>
#include <unordered_map>

using namespace nucc;

std::string chunk_type_to_string(chunk_type chunk_type) {
    switch (chunk_type) {
        case chunk_type::index                  : return "nuccChunkIndex";
        case chunk_type::null                   : return "nuccChunkNull";
        case chunk_type::page                   : return "nuccChunkPage";
        case chunk_type::texture                : return "nuccChunkTexture";
        case chunk_type::model                  : return "nuccChunkModel";
        case chunk_type::model_hit              : return "nuccChunkModelHit";
        case chunk_type::material               : return "nuccChunkMaterial";
        case chunk_type::camera                 : return "nuccChunkCamera";
        case chunk_type::anm                    : return "nuccChunkAnm";
        case chunk_type::billboard              : return "nuccChunkBillboard";
        case chunk_type::coord                  : return "nuccChunkCoord";
        case chunk_type::sprite                 : return "nuccChunkSprite";
        case chunk_type::particle               : return "nuccChunkParticle";
        case chunk_type::dynamics               : return "nuccChunkDynamics";
        case chunk_type::binary                 : return "nuccChunkBinary";
        case chunk_type::light_dirc             : return "nuccChunkLightDirc";
        case chunk_type::light_point            : return "nuccChunkLightPoint";
        case chunk_type::ambient                : return "nuccChunkAmbient";
        case chunk_type::sprite_anm             : return "nuccChunkSpriteAnm";
        case chunk_type::font                   : return "nuccChunkFont";
        case chunk_type::trail                  : return "nuccChunkTrail";
        case chunk_type::primitive_vertex       : return "nuccChunkPrimitiveVertex";
        case chunk_type::model_primitive_branch : return "nuccChunkModelPrimitiveBranch";
        case chunk_type::morph_primitive        : return "nuccChunkMorphPrimitive";
        case chunk_type::layer_set              : return "nuccChunkLayerSet";
        case chunk_type::model_vertex           : return "nuccChunkModelVertex";
        case chunk_type::morph_model            : return "nuccChunkMorphModel";
        case chunk_type::clump                  : return "nuccChunkClump";
        case chunk_type::light_set              : return "nuccChunkLightSet";
        case chunk_type::sprite2                : return "nuccChunkSprite2";
        case chunk_type::sprite2_anm            : return "nuccChunkSprite2Anm";
        case chunk_type::nub                    : return "nuccChunkNub";
    }
    return "nuccChunkUnknown";
}

chunk_type string_to_chunk_type(std::string_view str) {
    static const std::unordered_map<std::string_view, chunk_type> string_chunk_type_map = {
        {"nuccChunkIndex", chunk_type::index},
        {"nuccChunkNull", chunk_type::null},
        {"nuccChunkUnknown", chunk_type::unknown},
        {"nuccChunkPage", chunk_type::page},
        {"nuccChunkTexture", chunk_type::texture},
        {"nuccChunkModel", chunk_type::model},
        {"nuccChunkModelHit", chunk_type::model_hit},
        {"nuccChunkMaterial", chunk_type::material},
        {"nuccChunkCamera", chunk_type::camera},
        {"nuccChunkAnm", chunk_type::anm},
        {"nuccChunkBillboard", chunk_type::billboard},
        {"nuccChunkCoord", chunk_type::coord},
        {"nuccChunkSprite", chunk_type::sprite},
        {"nuccChunkParticle", chunk_type::particle},
        {"nuccChunkDynamics", chunk_type::dynamics},
        {"nuccChunkBinary", chunk_type::binary}
    };

    auto iterator = string_chunk_type_map.find(str);
    if (iterator != string_chunk_type_map.end())
        return iterator->second;
    return chunk_type::unknown;
}