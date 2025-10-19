#ifndef KOJO_NUCC_CHUNK_TEXTURE_HPP
#define KOJO_NUCC_CHUNK_TEXTURE_HPP

#include <nucc/chunk_meta.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class chunk_texture : public chunk_meta {
public:
        chunk_texture() = default;
        explicit chunk_texture(kojo::binary_view input)
        {
                chunk_texture::load(std::move(input));
        }
        size_t load(kojo::binary_view input) override;

        [[nodiscard]] std::uint16_t width() const { return m_width; }
        [[nodiscard]] std::uint16_t height() const { return m_height; }
        [[nodiscard]] std::uint32_t size() const { return m_size; }

        void update(kojo::binary_view data) override {}

private:
        std::uint16_t m_width{}, m_height{};
        std::uint32_t m_size{};
};

}

#endif