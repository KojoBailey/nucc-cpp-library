#include <nucc/chunks/binary/asbr/PlayerColorParam.hpp>

#include <format>

using namespace nucc::asbr;

std::string PlayerColorParam::Entry::key() {
    return std::format("{}{}{}col{}", character_id.substr(0, 4), costume_index, character_id.at(5), tint_index);
}

void PlayerColorParam::read_bytes(const std::byte* src, const size_t size = 0) {
    if (input == nullptr) return error_handler({
        nucc::Status_Code::POINTER_NULL,
        "Attempted to load PlayerColorParam chunk data, but received null input.",
        "Ensure the provided data is being passed correctly."
    });
    storage.load(input, 0, size_input);

    version = storage.read_int<std::uint32_t>(std::endian::little);
    entry_count = storage.read_int<std::uint32_t>(std::endian::little);
    first_pointer = storage.read_int<std::uint64_t>(std::endian::little);
    storage.change_pos(first_pointer - 8);

    Entry entry_buffer;
    for (int i = 0; i < entry_count; i++) {
        ptr_buffer64                     = storage.read_int<std::uint64_t>(std::endian::little);
        entry_buffer.character_id      = storage.read_str(0, ptr_buffer64 - 8);
        entry_buffer.costume_index     = storage.read_int<std::uint32_t>(std::endian::little);
        entry_buffer.color.red         = storage.read_int<std::uint32_t>(std::endian::little);
        entry_buffer.color.green       = storage.read_int<std::uint32_t>(std::endian::little);
        entry_buffer.color.blue        = storage.read_int<std::uint32_t>(std::endian::little);

        entry_buffer.tint_index = tint_tracker[std::format("{}-{}", entry_buffer.character_id, entry_buffer.costume_index)]++;

        entries[entry_buffer.key()] = entry_buffer;
    }

    return 0;
}