#ifndef KOJO_NUCC_CHUNK_PAGE_HPP
#define KOJO_NUCC_CHUNK_PAGE_HPP

#include <nucc/chunk_meta.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class chunk_page : public chunk_meta {
public:
        chunk_page() = default;
        explicit chunk_page(kojo::binary_view input) {
                chunk_page::load(std::move(input));
        }
        size_t load(kojo::binary_view input) override;

        [[nodiscard]] std::uint32_t chunk_map_offset() const { return m_chunk_map_offset; }
        [[nodiscard]] std::uint32_t extra_map_offset() const { return m_extra_map_offset; }

        void update(kojo::binary_view) override {}

private:
        std::uint32_t m_chunk_map_offset{};
        std::uint32_t m_extra_map_offset{};
};

}

#endif