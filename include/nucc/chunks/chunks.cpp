#include "chunks.hpp"

namespace nucc {

std::unordered_map<std::string, Chunk_Type> string_to_chunk_type_table{
    {"nuccChunkIndex", Chunk_Type::Index},
    {"nuccChunkNull", Chunk_Type::Null},
    {"nuccChunkUnknown", Chunk_Type::Unknown},
    {"nuccChunkPage", Chunk_Type::Page},
    {"nuccChunkTexture", Chunk_Type::Texture},
    {"nuccChunkModel", Chunk_Type::Model},
    {"nuccChunkModelHit", Chunk_Type::ModelHit},
    {"nuccChunkMaterial", Chunk_Type::Material},
    {"nuccChunkCamera", Chunk_Type::Camera},
    {"nuccChunkAnm", Chunk_Type::Anm},
    {"nuccChunkBillboard", Chunk_Type::Billboard},
    {"nuccChunkCoord", Chunk_Type::Coord},
    {"nuccChunkSprite", Chunk_Type::Sprite},
    {"nuccChunkParticle", Chunk_Type::Particle},
    {"nuccChunkDynamics", Chunk_Type::Dynamics},
    {"nuccChunkBinary", Chunk_Type::Binary}
};

Chunk_Type string_to_chunk_type(std::string str) {
    if (string_to_chunk_type_table.contains(str))
        return string_to_chunk_type_table[str];
    return Chunk_Type::Unknown;
}

} // namespace nucc