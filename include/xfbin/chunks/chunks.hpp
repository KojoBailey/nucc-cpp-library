#include "../external/binary-lib/binary/binary.hpp"

namespace nucc {

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

// #include "chunks/anm.hpp"
// #include "binary.hpp"
// #include "chunks/clump.hpp"
// #include "chunks/index.hpp"
// #include "chunks/model.hpp"
// #include "chunks/nub.hpp"
// #include "chunks/page.hpp"
// #include "chunks/texture.hpp"

class Chunk {
public:
    ChunkType type{ChunkType::Null};    /** @warning Must match the struct used (e.g. `Binary` for nuccChunkBinary). */
    std::string path;                   /** Internal chunk file path. @note Can usually be used to uniquely ID chunks. */
    std::string name;                   /** Chunk name. @warning Typically the same across game versions, so not reliable for IDs. */

    struct {
        std::uint32_t size{0};          /** Size of entire chunk in bytes. */
        std::uint32_t map_index;
        std::uint16_t version{121};     /** May differ from XFBIN version. */
        std::uint16_t unk;              /** @note Unknown, but may be animation-related. */
    } Meta;

    Chunk() {};
    Chunk(ChunkType type_input, std::string_view path_input, std::string_view name_input)
        : type(type_input), path(path_input), name(name_input) {}

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

    virtual void load(kojo::binary& input) {}
    virtual kojo::binary dump() { return 0; }
};

}