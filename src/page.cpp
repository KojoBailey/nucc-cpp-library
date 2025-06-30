#include <nucc/page_new.hpp>

using namespace nucc;

void Page::add(Chunk chunk) {
    chunks.push_back(chunk);
}

const Chunk& Page::fetch(size_t index) const {
    return chunks[index];
}
const Chunk& Page::fetch(ChunkType type, size_t index = 0) const {
    size_t i = 0;
    for (const Chunk& chunk : chunks) {
        if (chunk.type() == type) {
            if (i == index)
                return chunk;
            i++;
        }
    }
    return fetch_last();
}
const Chunk& Page::fetch(std::string_view name, size_t index = 0) const {
    size_t i = 0;
    for (const Chunk& chunk : chunks) {
        if (chunk.path() == name || chunk.name() == name) {
            if (i == index)
                return chunk;
            i++;
        }
    }
    return fetch_last();
}

const Chunk& Page::fetch_last() const {
    return chunks[chunks.size() - 1];
}