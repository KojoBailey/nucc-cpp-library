#include <nucc/xfbin.hpp>
#include <nucc/chunk_null.hpp>

using namespace nucc;
using namespace kojo::binary_types;

chunk::chunk(kojo::binary_view& input_data, xfbin* xfbin) {
    load(input_data, xfbin);
}

void chunk::load(kojo::binary_view& input_data, xfbin* xfbin) {
    if (input_data.is_empty()) {
        log.error(
            kojo::logger::status::null_pointer,
            "Data passed to chunk is null.",
            "Ensure the data being passed is not a null memory."
        );
        return;
    }

    auto size = input_data.read<u32>(std::endian::big);
    auto map_index = input_data.read<u32>(std::endian::big);
    m_version = input_data.read<u16>(std::endian::big);
    input_data.read<u16>(std::endian::big); // Potentially for animations.

    m_type = xfbin->get_type(map_index);
    m_path = xfbin->get_path(map_index);
    m_name = xfbin->get_name(map_index);

    log.debug(std::format(R"(Type: {}, Name: "{}", Path: "{}")", chunk_type_to_string(m_type), m_name, m_path));

    switch (m_type) {
        case chunk_type::page:
            m_meta = std::make_shared<chunk_page>();
            break;
        case chunk_type::binary:
            m_meta = std::make_shared<chunk_binary>();
            break;
        case chunk_type::texture:
            m_meta = std::make_shared<chunk_texture>();
            break;
        default:
            m_meta = std::make_shared<chunk_null>();
    }

    // Technically the original implementation (ASBR) decrypts first the inner size of the nuccChunkBinary,
    // then rest of the data, but since the size is always 4 bytes (which fits perfectly with the decryption alignment),
    // we can just decrypt everything at once without any issues.
    // TODO: maybe move this into the chunk_binary class?
    if (m_type == chunk_type::binary && xfbin->should_decrypt && size > 0) {
        log.verbose(std::format("Decrypting rest of chunk data... ({} bytes)", size));
        uint8_t *data_ptr = (uint8_t*)input_data.data() + input_data.get_pos();
        xfbin->cryptor.decrypt(data_ptr, data_ptr, size);
    }

    size_t meta_size = m_meta->load(input_data);

    m_data.load(input_data.data(), size - meta_size, input_data.get_pos() + meta_size);
    input_data.change_pos(size);
}

void chunk::update_data(const std::byte* _data, size_t size) {
    m_data.load(_data, size);
    m_meta->update(m_data.data());
}