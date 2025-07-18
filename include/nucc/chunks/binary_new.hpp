#ifndef KOJO_NUCC_BINARY
#define KOJO_NUCC_BINARY

#include <nucc/chunks/chunk_meta.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class binary : chunk_meta {
public:
    binary() = default;
    binary(const std::byte* input, size_t position);

    void load(const std::byte* input, size_t position);

    std::uint32_t size() const { return m_size; }

private:
    std::uint32_t m_size;
};

}

#endif // KOJO_NUCC_BINARY