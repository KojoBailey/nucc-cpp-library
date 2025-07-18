#include <nucc/chunks/binary/asbr/player_color_param.hpp>

#include <nucc/error_handler.hpp>

using namespace nucc::asbr;

using namespace kojo::binary_types;

std::string player_color_param::entry::key() {
    return std::format("{}{}{}col{}", character_id.substr(0, 4), costume_index, character_id.at(5), tint_index);
}

size_t player_color_param::size() const {
    return HEADER_SIZE + entries.size() * (ENTRY_SIZE + CHARACTER_ID_LENGTH);
}

void player_color_param::read_binary(const std::byte* src, const size_t start = 0) {
    if (src == nullptr) return error::print(
        nucc::status_code::null_pointer,
        "Attempted to load PlayerColorParam chunk data, but received null input.",
        "Ensure the provided data is being passed correctly."
    );
    kojo::binary_view data{src, start};

    auto VERSION = data.read<u32>(std::endian::little);
    auto entry_count = data.read<u32>(std::endian::little);
    auto first_pointer = data.read<u64>(std::endian::little);
    data.change_pos(first_pointer - 8);

    entry entry_buffer;
    for (int i = 0; i < entry_count; i++) {
        auto character_id_pointer   = data.read<u64>(std::endian::little);
        entry_buffer.character_id   = data.read<sv>(character_id_pointer - 8);
        entry_buffer.costume_index  = data.read<u32>(std::endian::little);
        entry_buffer.color.red      = data.read<u32>(std::endian::little);
        entry_buffer.color.green    = data.read<u32>(std::endian::little);
        entry_buffer.color.blue     = data.read<u32>(std::endian::little);

        std::string tint_id = std::format("{}-{}", entry_buffer.character_id, entry_buffer.costume_index);
        entry_buffer.tint_index = tint_tracker[tint_id]++;

        entries[entry_buffer.key()] = entry_buffer;
    }
}

void player_color_param::read_json(const nlohmann::ordered_json& input) {
    for (const auto& [key, value] : input.items()) {
        if (key == "Version" || key == "Filetype") continue;

        if (!value.is_string()) return error::print(
            nucc::status_code::json_value,
            std::format("JSON data for entry \"{}\" is not a valid hex code.", key),
            "Ensure all hex codes are strings with the format \"#RRGGBB\"."
        );
        if (value.template get<std::string>().length() != 7) return error::print(
            nucc::status_code::json_value,
            std::format("JSON data for entry \"{}\" is not a valid hex code.", key),
            "Ensure all hex codes are strings with the format \"#RRGGBB\". Alpha channel is not supported."
        );

        entry entry_buffer;
        entry_buffer.character_id = key.substr(0, 4) + "0" + key.at(5);
        entry_buffer.costume_index = key.at(4) - '0';
        entry_buffer.color.from_hex_code(value);
        entry_buffer.tint_index = key.at(9);

        entries[key] = entry_buffer;
    }
}

const std::shared_ptr<std::vector<std::byte>> player_color_param::write_binary() const {
    kojo::binary output_data;

    const u32 entry_count = entries.size();
    const u64 first_pointer = 8;
    output_data.write<u32>(VERSION, std::endian::little);
    output_data.write<u32>(entry_count, std::endian::little);
    output_data.write<u64>(first_pointer, std::endian::little);

    size_t i = 0;
    for (auto& [key, entry] : entries) {
        auto character_id_pointer = ENTRY_SIZE * entry_count + i * CHARACTER_ID_LENGTH;
        output_data.write<u64>(character_id_pointer, std::endian::little);
        output_data.write<u32>(entry.costume_index, std::endian::little);
        output_data.write<u32>(entry.color.red, std::endian::little);
        output_data.write<u32>(entry.color.green, std::endian::little);
        output_data.write<u32>(entry.color.blue, std::endian::little);
    }
    for (auto& [key, entry] : entries) {
        output_data.write<sv>(entry.character_id, CHARACTER_ID_LENGTH);
    }

    return output_data.storage();
}

nlohmann::ordered_json player_color_param::write_json() {
    nlohmann::ordered_json json;

    json["Version"] = 250718;
    json["Filetype"] = "PlayerColorParam";

    for (auto& [key, entry] : entries) {
        json[key] = entry.color.to_hex_code();
    }

    return json;
}