#ifndef KOJO_NUCC_CHUNK_TYPE
#define KOJO_NUCC_CHUNK_TYPE

#include <string>
#include <string_view>

namespace nucc {

enum class chunk_type {
    index = 0,              // Supported
    null,                   // Supported
    unknown,
    page,                   // Supported
    texture,                // WIP
    model,
    model_hit,
    material,
    camera,
    anm,
    billboard,
    coord,
    sprite,
    particle,
    dynamics,
    binary,                 // WIP
    light_dirc,
    light_point,
    ambient,
    sprite_anm,
    font,
    trail,
    primitive_vertex,
    model_primitive_branch,
    morph_primitive,
    layer_set,
    model_vertex,
    morph_model,
    clump,
    light_set,
    sprite2,
    sprite2_anm,
    nub,
};

std::string chunk_type_to_string(chunk_type);
chunk_type string_to_chunk_type(std::string_view);

}

#endif // KOJO_NUCC_CHUNK_TYPE