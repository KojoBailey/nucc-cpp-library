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
    static constexpr Chunk_Type type{Chunk_Type::Page};
    static constexpr std::string_view name{"Page0"};

    struct {
        std::uint32_t map_offset;
        std::uint32_t extra_offset;
    } content;

    std::vector<Chunk> chunks;

    Page() = default;
    Page(Chunk* chunk) {
        load(chunk);
    }

    Chunk* create_chunk(Chunk_Type type = Chunk_Type::Null, std::string_view path = "", std::string_view name = "") {
        chunks.emplace_back();
        auto& chunk = chunks[chunks.size() - 1];
        chunk.type = type;
        chunk.path = path;
        chunk.name = name;
        return &chunk;
    }

    kojo::binary& dump() {
        output.clear();
        output.write_int<std::uint32_t>(content.map_offset, std::endian::big);
        output.write_int<std::uint32_t>(content.extra_offset, std::endian::big);
        for (auto& chunk : chunks) {
            output.write_binary(chunk.dump());
        }
        return output;
    }

private:
    void parse() {
        storage.set_pos(0);
        content.map_offset = storage.read_int<std::uint32_t>(std::endian::big);
        content.extra_offset = storage.read_int<std::uint32_t>(std::endian::big);
    }
};

}

#endif // KOJO_NUCC_PAGE