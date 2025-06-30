#ifndef KOJO_NUCC_PAGE
#define KOJO_NUCC_PAGE

#include <nucc/chunk_types.hpp>

#include <vector>

namespace nucc {

class Chunk;

class Page {
public:
    Page() = default;

    void add(Chunk chunk);

    const Chunk& fetch(size_t index) const;
    const Chunk& fetch(ChunkType type, size_t index = 0) const;
    const Chunk& fetch(std::string_view name, size_t index = 0) const;

private:
    std::vector<Chunk> chunks;

    const Chunk& fetch_last() const;
};

}

#endif // KOJO_NUCC_PAGE