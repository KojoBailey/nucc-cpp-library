#ifndef KOJO_NUCC_PAGE
#define KOJO_NUCC_PAGE

#include <nucc/chunk_type.hpp>
#include <nucc/chunk.hpp>

#include <vector>

namespace nucc {

class chunk;

class page {
public:
    page() = default;

    std::uint32_t version() const { return m_version; }

    std::uint32_t map_offset() const { return m_map_offset; }
    std::uint32_t extra_offset() const { return m_extra_offset; }

    const std::vector<chunk>& chunks() const { return m_chunks; }

    const chunk& get_chunk(size_t index) const;
    const chunk& get_chunk(chunk_type type, size_t index = 0) const;
    const chunk& get_chunk(std::string_view name, size_t index = 0) const;

    bool has(std::string_view chunk_name) const;
    bool has(chunk_type chunk_type) const;

    void add_chunk(chunk& chunk);

    std::vector<chunk>::const_iterator end() const { return m_chunks.end(); }

private:
    std::vector<chunk> m_chunks;

    std::uint32_t m_version;

    std::uint32_t m_map_offset;
    std::uint32_t m_extra_offset;

    const chunk& fetch_last() const;
};

}

#endif // KOJO_NUCC_PAGE