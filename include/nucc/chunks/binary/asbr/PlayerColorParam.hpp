#ifndef KOJO_NUCC_ASBR_PLAYERCOLORPARAM
#define KOJO_NUCC_ASBR_PLAYERCOLORPARAM

#include "../binary_data.hpp"

namespace nucc {
    namespace ASBR {
        
class PlayerColorParam : public Binary_Data {
public:
    struct Entry {
        std::string character_id; // Length always 6.
        std::uint32_t tint_index; // Not written.
        std::uint32_t costume_index;
        RGB color;

        std::string key() {
            return std::format("{}{}{}col{}", character_id.substr(0, 4), costume_index, character_id.at(5), tint_index);
        }
    };

    std::uint32_t version;
    std::uint32_t entry_count;
    std::uint64_t first_pointer;
    std::map<std::string, Entry> entries;

    PlayerColorParam(void* input, size_t size_input = -1) {
        load(input, size_input);
    }

    int load(void* input, size_t size_input = -1) {
        if (input == nullptr) return error_handler({
            nucc::Status_Code::POINTER_NULL,
            "Attempted to load PlayerColorParam chunk data, but received null input.",
            "Ensure the provided data is being passed correctly."
        });
        storage.load(input, 0, size_input);

        version = storage.read<std::uint32_t>(kojo::endian::little);
        entry_count = storage.read<std::uint32_t>(kojo::endian::little);
        first_pointer = storage.read<std::uint64_t>(kojo::endian::little);
        storage.change_pos(first_pointer - 8);

        Entry entry_buffer;
        for (int i = 0; i < entry_count; i++) {
            ptr_buffer64                     = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.character_id      = storage.read<std::string>(0, ptr_buffer64 - 8);
            entry_buffer.costume_index     = storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.color.red         = storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.color.green       = storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.color.blue        = storage.read<std::uint32_t>(kojo::endian::little);

            entry_buffer.tint_index = tint_tracker[std::format("{}-{}", entry_buffer.character_id, entry_buffer.costume_index)]++;

            entries[entry_buffer.key()] = entry_buffer;
        }

        return 0;
    }
    int load(nlohmann::ordered_json input) {
        if (input.is_null()) return 0;

        for (const auto& [key, value] : input.items()) {
            if (!value.is_string()) continue;

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
        storage.write<std::uint32_t>(version, kojo::endian::little);
        storage.write<std::uint32_t>(entry_count, kojo::endian::little);
        storage.write<std::uint64_t>(first_pointer, kojo::endian::little);

        size_t i = 0;
        for (auto& [key, entry] : entries) {
            ptr_buffer64 = (24 * entry_count) + (i * 7) - (i++ * 2);
            storage.write<std::uint64_t>(ptr_buffer64, kojo::endian::little);
            storage.write<std::uint32_t>(entry.costume_index, kojo::endian::little);
            storage.write<std::uint32_t>(entry.color.red, kojo::endian::little);
            storage.write<std::uint32_t>(entry.color.green, kojo::endian::little);
            storage.write<std::uint32_t>(entry.color.blue, kojo::endian::little);
        }
        for (auto& [key, entry] : entries) {
            storage.write<std::string>(entry.character_id);
        }

        return (std::uint64_t*)storage.data();
    }
    std::string write_to_json() {
        nlohmann::ordered_json json;
        
        for (auto& [key, entry] : entries) {
            json[key] = entry.color.rgb_to_hex();
        }

        return json.dump(2);
    }

    std::unordered_map<std::string, int> tint_tracker;
};

    } // namespace ASBR
} // namespace nucc

#endif // KOJO_NUCC_ASBR_PLAYERCOLORPARAM