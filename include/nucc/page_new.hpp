#ifndef KOJO_NUCC_PAGE
#define KOJO_NUCC_PAGE

#include <nucc/chunk_type.hpp>
#include <nucc/chunk_new.hpp>

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

    const std::vector<Chunk>& chunks() const { return m_chunks; }

    std::uint32_t version() const { return m_version; }

    std::uint32_t map_offset() const { return m_map_offset; }
    std::uint32_t extra_offset() const { return m_extra_offset; }

private:
    std::vector<Chunk> m_chunks;

    std::uint32_t m_version;

    std::uint32_t m_map_offset;
    std::uint32_t m_extra_offset;

    const Chunk& fetch_last() const;
};

}

#endif // KOJO_NUCC_PAGE