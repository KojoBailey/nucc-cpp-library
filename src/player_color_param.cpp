#include <nucc/chunks/binary/asbr/player_color_param.hpp>

#include <format>

using namespace nucc::asbr;

std::string player_color_param::entry::key() {
    return std::format("{}{}{}col{}", character_id.substr(0, 4), costume_index, character_id.at(5), tint_index);
}

void player_color_param::read_binary(const std::byte* src, const size_t size = 0) {
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

void player_color_param::read_json(const nlohmann::ordered_json& input) {

    for (const auto& [key, value] : input.items()) {
        if (key == "Version" || key == "Filetype") continue;

        if (!value.is_string()) return error_handler({
            nucc::Status_Code::JSON_VALUE,
            std::format("JSON data for entry \"{}\" is not a valid hex code.", key),
            "Ensure all hex codes are strings with the format \"#RRGGBB\"."
        });
        if (value.template get<std::string>().length() != 7) return error_handler({
            nucc::Status_Code::JSON_VALUE,
            std::format("JSON data for entry \"{}\" is not a valid hex code.", key),
            "Ensure all hex codes are strings with the format \"#RRGGBB\". Alpha channel is not supported."
        });

        Entry entry_buffer;
        entry_buffer.character_id = key.substr(0, 4) + "0" + key.at(5);
        entry_buffer.costume_index = key.at(4) - '0';
        entry_buffer.color.hex_to_rgb(value);
        entry_buffer.tint_index = key.at(9);

        entries[key] = entry_buffer;
    }

    return 0;
}