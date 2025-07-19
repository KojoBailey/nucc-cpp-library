#include <nucc/chunk_new.hpp>
#include <nucc/xfbin_new.hpp>

using namespace nucc;
using namespace kojo::binary_types;

chunk::chunk(const std::byte* input, size_t position, const xfbin* xfbin) {
    load(input, position, xfbin);
}

void chunk::load(const std::byte* input, size_t position, const xfbin* xfbin) {
    kojo::binary_view input_data{input, position};
    if (input_data.is_empty()) return;

    auto size = input_data.read<u32>(std::endian::big);
    auto map_index = input_data.read<u32>(std::endian::big);
    m_version = input_data.read<u16>(std::endian::big);
    auto unk = input_data.read<u16>(std::endian::big); // Potentially for animations.

    m_type = xfbin->get_type(map_index);
    m_path = xfbin->get_path(map_index);
    m_name = xfbin->get_name(map_index);

    size_t meta_size = m_meta->load(input_data);

    m_data.load(input_data.data(), size - meta_size, input_data.get_pos());
}