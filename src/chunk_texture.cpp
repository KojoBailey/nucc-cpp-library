#include <nucc/chunk_texture.hpp>

using namespace nucc;
using namespace kojo::binary_types;

size_t chunk_texture::load(kojo::binary_view input) {
    size_t starting_pos = input.get_pos();

    input.change_pos(2); // unk00
    m_width = input.read<u16>(std::endian::big);
    m_height = input.read<u16>(std::endian::big);
    input.change_pos(2); // unk06
    m_size = input.read<u32>(std::endian::big);

    return input.get_pos() - starting_pos;
}