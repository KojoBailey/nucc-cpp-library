#ifndef KOJO_NUCC_ASBR_PLAYERCOLORPARAM
#define KOJO_NUCC_ASBR_PLAYERCOLORPARAM

#include <nucc/chunks/binary/bytes_serializable.hpp>
#include <nucc/chunks/binary/json_serializable.hpp>

#include <nucc/rgb.hpp>

namespace nucc {
    namespace asbr {
        
class PlayerColorParam : public bytes_serializable, json_serializable {
public:
    struct Entry {
        std::string character_id; // Length always 6.
        std::uint32_t tint_index; // Not written.
        std::uint32_t costume_index;
        rgb color;

        std::string key();
    };

    std::uint32_t version;
    std::uint32_t entry_count;
    std::uint64_t first_pointer;
    std::map<std::string, Entry> entries;

    static constexpr std::string_view path() { return "PlayerColorParam.bin"; }

    void read_bytes(const std::byte* src, const size_t size = 0) override {
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
    int load(nlohmann::ordered_json& input) {
        if (input.is_null()) return 0;

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

    size_t size() {
        return entry_count * 31 + 8 + first_pointer;
    }
    void clear() {
        version = 1000;
        entry_count = 0;
        first_pointer = 8;
        entries.clear();
        tint_tracker.clear();
    }

    std::uint64_t* write_to_bin() {
        version = 1000;
        entry_count = entries.size();
        storage.write_int<std::uint32_t>(version, std::endian::little);
        storage.write_int<std::uint32_t>(entry_count, std::endian::little);
        storage.write_int<std::uint64_t>(first_pointer, std::endian::little);

        size_t i = 0;
        for (auto& [key, entry] : entries) {
            ptr_buffer64 = (24 * entry_count) + (i * 7) - (i++ * 2);
            storage.write_int<std::uint64_t>(ptr_buffer64, std::endian::little);
            storage.write_int<std::uint32_t>(entry.costume_index, std::endian::little);
            storage.write_int<std::uint32_t>(entry.color.red, std::endian::little);
            storage.write_int<std::uint32_t>(entry.color.green, std::endian::little);
            storage.write_int<std::uint32_t>(entry.color.blue, std::endian::little);
        }
        for (auto& [key, entry] : entries) {
            storage.write_str(entry.character_id);
        }

        return (std::uint64_t*)storage.data();
    }
    nlohmann::ordered_json write_to_json() {
        nlohmann::ordered_json json;

        json["Version"] = 241001;
        json["Filetype"] = "PlayerColorParam";
        
        for (auto& [key, entry] : entries) {
            json[key] = entry.color.rgb_to_hex();
        }

        return json;
    }

    std::unordered_map<std::string, int> tint_tracker;
};

    } // namespace ASBR
} // namespace nucc

#endif // KOJO_NUCC_ASBR_PLAYERCOLORPARAM