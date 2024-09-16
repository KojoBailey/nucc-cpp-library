#ifndef KOJO_NUCC_ASBR_PLAYERCOLORPARAM
#define KOJO_NUCC_ASBR_PLAYERCOLORPARAM

#include "../binary_data.hpp"

namespace nucc {
    namespace ASBR {
        
class PlayerColorParam : public Binary_Data {
public:
    struct Entry {
        std::uint64_t character_id_ptr;
        std::string character_id; // Length always 6.
        std::uint32_t tint_index; // Not written.
        std::uint32_t costume_index;
        std::uint32_t red, green, blue;
    };

    std::uint32_t version;
    std::uint32_t entry_count;
    std::uint64_t first_pointer;
    std::vector<Entry> entries;

    PlayerColorParam(void* input, size_t size_input = -1) {
        load(input, size_input);
    }

    void load(void* input, size_t size_input = -1) {
        storage.load(input, 0, size_input);

        version = storage.read<std::uint32_t>(kojo::endian::little);
        entry_count = storage.read<std::uint32_t>(kojo::endian::little);
        first_pointer = storage.read<std::uint64_t>(kojo::endian::little);
        storage.change_pos(first_pointer - 8);
        for (int i = 0; i < entry_count; i++) {
            entries.push_back({});
            auto& entry = entries[i];

            entry.character_id_ptr  = storage.read<std::uint64_t>(kojo::endian::little);
            entry.character_id      = storage.read<std::string>(0, entry.character_id_ptr - 8);
            entry.costume_index     = storage.read<std::uint32_t>(kojo::endian::little);
            entry.red               = storage.read<std::uint32_t>(kojo::endian::little);
            entry.green             = storage.read<std::uint32_t>(kojo::endian::little);
            entry.blue              = storage.read<std::uint32_t>(kojo::endian::little);

            entry.tint_index = tint_tracker[std::format("{}-{}", entry.character_id, entry.costume_index)]++;
        }
    }
    void load(nlohmann::ordered_json input) {

    }

    size_t size() {
        return entry_count * 31 + 16;
    }
    void clear() {
        version = 0;
        entry_count = 0;
        first_pointer = 8;
        entries.clear();
        tint_tracker.clear();
    }

    kojo::binary& write_to_bin() {
        version = 1000;
        entry_count = entries.size();
        storage.write<std::uint32_t>(version, kojo::endian::little);
        storage.write<std::uint32_t>(entry_count, kojo::endian::little);
        storage.write<std::uint64_t>(first_pointer, kojo::endian::little);

        size_t i{0};
        for (auto& entry : entries) {
            entry.character_id_ptr = (24 * entry_count) + (i * 7) - (i++ * 2);
            storage.write<std::uint64_t>(entry.character_id_ptr, kojo::endian::little);
            storage.write<std::uint32_t>(entry.costume_index, kojo::endian::little);
            storage.write<std::uint32_t>(entry.red, kojo::endian::little);
            storage.write<std::uint32_t>(entry.green, kojo::endian::little);
            storage.write<std::uint32_t>(entry.blue, kojo::endian::little);
        }
        for (auto& entry : entries) {
            storage.write<std::string>(entry.character_id);
        }

        return storage;
    }
    std::string write_to_json() {
        nlohmann::ordered_json json;
        std::string key_buffer;
        
        for (auto& entry : entries) {
            key_buffer = std::format("{}0{}col{}", entry.character_id.substr(0, 4), entry.costume_index, entry.tint_index);
            json[key_buffer] = std::format("#{:02x}{:02x}{:02x}", entry.red, entry.green, entry.blue);
        }

        return json.dump(2);
    }

private:
    std::unordered_map<std::string, int> tint_tracker;
};

    } // namespace ASBR
} // namespace nucc

#endif // KOJO_NUCC_ASBR_PLAYERCOLORPARAM