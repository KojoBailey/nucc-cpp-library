#ifndef KOJO_NUCC_CHUNK_TEXTURE
#define KOJO_NUCC_CHUNK_TEXTURE

#include <nucc/chunks/chunk_meta.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class chunk_texture : public chunk_meta {
public:
    size_t load(kojo::binary_view input) override;

    std::uint16_t width() const { return m_width; }
    std::uint16_t height() const { return m_height; }
    std::uint32_t size() const { return m_size; }

private:
    std::uint16_t m_width, m_height;
    std::uint32_t m_size;
};

}

#endif // KOJO_NUCC_CHUNK_TEXTURE