#include "chunks.hpp"
#include "texture/nut.hpp"

namespace nucc {

class Texture : public Chunk {
    struct {
        std::uint16_t unk0;
        std::uint16_t width, height;
        std::uint16_t unk6;
        std::uint32_t size;
        NUT nut;
    } content;

    Texture() {
        type = Chunk_Type::Texture;
    }
};

}