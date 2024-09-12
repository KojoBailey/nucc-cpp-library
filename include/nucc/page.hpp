#ifndef KOJO_NUCC_PAGE
#define KOJO_NUCC_PAGE

#include "chunks/chunks.hpp"

namespace nucc {
    
/**
 * Separates chunks into distinct groups.
 * @note Might be because of clumps.
*/
class Page : public Chunk {
public:
    Chunk_Type type{Chunk_Type::Page};
    std::string name{"Page0"};

    struct {
        std::uint32_t map_offset;
        std::uint32_t extra_offset;
    } content;

    std::vector<Chunk> chunks;

    Page() = default;
    Page(Chunk* chunk) {
        load(chunk);
    }

    kojo::binary& dump() {
        output.clear();
        output.write<std::uint32_t>(content.map_offset, kojo::endian::big);
        output.write<std::uint32_t>(content.extra_offset, kojo::endian::big);
        for (auto& chunk : chunks) {
            output.write<kojo::binary>(chunk.dump());
        }
        return output;
    }

private:
    void parse() {
        storage.set_pos(0);
        content.map_offset = storage.read<std::uint32_t>(kojo::endian::big);
        content.extra_offset = storage.read<std::uint32_t>(kojo::endian::big);
    }
};

}

#endif // KOJO_NUCC_PAGE