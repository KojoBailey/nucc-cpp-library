#include <nucc/chunk_page.hpp>

using namespace nucc;
using namespace kojo::binary_types;

size_t chunk_page::load(kojo::binary_view input) {
    size_t starting_pos = input.get_pos();
    m_chunk_map_offset = input.read<u32>(std::endian::big);
    m_extra_map_offset = input.read<u32>(std::endian::big);
    return input.get_pos() - starting_pos;
}