#include <nucc/chunks_new.hpp>

using namespace nucc;
using namespace kojo::binary_types;

Chunk::Chunk(const std::byte* input, size_t position, const XFBIN* xfbin) {
    load(input, position, xfbin);
}

void Chunk::load(const std::byte* input, size_t position, const XFBIN* xfbin) {
    kojo::binary_view input_data{input, position};
    if (input_data.is_empty()) return;

    auto size = input_data.read<u32>(std::endian::big);
    auto map_index = input_data.read<u32>(std::endian::big);
    m_version = input_data.read<u16>(std::endian::big);
    auto unk = input_data.read<u16>(std::endian::big); // Potentially for animations.

    m_type = xfbin->get_type(map_index);
    m_path = xfbin->get_path(map_index);
    m_name = xfbin->get_name(map_index);

    data.load(input_data.data(), size, input_data.get_pos());
}