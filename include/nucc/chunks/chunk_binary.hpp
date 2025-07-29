#ifndef KOJO_NUCC_CHUNK_BINARY
#define KOJO_NUCC_CHUNK_BINARY

#include <nucc/chunks/chunk_meta.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class chunk_binary : public chunk_meta {
public:
    size_t load(kojo::binary_view input) override;

    std::uint32_t size() const { return m_size; }

private:
    std::uint32_t m_size;
};

}

#endif // KOJO_NUCC_CHUNK_BINARY