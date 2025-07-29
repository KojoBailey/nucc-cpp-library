#include <nucc/chunk_binary.hpp>

using namespace nucc;
using namespace kojo::binary_types;

size_t chunk_binary::load(kojo::binary_view input) {
    size_t starting_pos = input.get_pos();
    m_size = input.read<u32>(std::endian::big);
    return input.get_pos() - starting_pos;
}