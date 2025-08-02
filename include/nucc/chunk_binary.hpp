#ifndef KOJO_NUCC_CHUNK_BINARY
#define KOJO_NUCC_CHUNK_BINARY

#include <nucc/chunk_meta.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class chunk_binary : public chunk_meta {
public:
    chunk_binary() = default;
    explicit chunk_binary(kojo::binary_view input) {
        chunk_binary::load(std::move(input));
    }
    size_t load(kojo::binary_view input) override;

    [[nodiscard]] std::uint32_t size() const { return m_size; }

private:
    std::uint32_t m_size{};
};

}

#endif