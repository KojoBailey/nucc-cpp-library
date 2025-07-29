#ifndef KOJO_NUCC_CHUNK_PAGE
#define KOJO_NUCC_CHUNK_PAGE

#include <nucc/chunk_meta.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class chunk_page : public chunk_meta {
public:
    size_t load(kojo::binary_view input) override;

    std::uint32_t chunk_map_offset() const { return m_chunk_map_offset; }
    std::uint32_t extra_map_offset() const { return m_extra_map_offset; }

private:
    std::uint32_t m_chunk_map_offset;
    std::uint32_t m_extra_map_offset;
};

}

#endif // KOJO_NUCC_CHUNK_PAGE