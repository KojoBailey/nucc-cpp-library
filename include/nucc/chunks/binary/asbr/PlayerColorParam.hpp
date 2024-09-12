#ifndef KOJO_NUCC_ASBR_PLAYERCOLORPARAM
#define KOJO_NUCC_ASBR_PLAYERCOLORPARAM

#include "../binary_data.hpp"

namespace nucc {
    namespace ASBR {
        
class PlayerColorParam : public Binary_Data {
public:
    struct Entry {
        std::uint64_t character_id_ptr;
        std::string character_id;
        std::uint32_t costume_index;
        std::uint32_t red, green, blue;
    };

    std::uint32_t version;
    std::uint32_t entry_count;
    std::uint64_t first_pointer;
    std::vector<Entry> entries;

    PlayerColorParam(void* input, size_t size_input = -1) {
        storage.load(input, 0, size_input);
        version = storage.read<std::uint32_t>(kojo::endian::little);
        entry_count = storage.read<std::uint32_t>(kojo::endian::little);
        first_pointer = storage.read<std::uint64_t>(kojo::endian::little);
        storage.change_pos(first_pointer - 8);
        for (int i = 0; i < entry_count; i++) {
            entries.push_back({});
            entries[i].character_id_ptr = storage.read<std::uint64_t>(kojo::endian::little);
            entries[i].character_id = storage.read<std::string>(0, entries[i].character_id_ptr - 8);
            entries[i].costume_index = storage.read<std::uint32_t>(kojo::endian::little);
            entries[i].red = storage.read<std::uint32_t>(kojo::endian::little);
            entries[i].green = storage.read<std::uint32_t>(kojo::endian::little);
            entries[i].blue = storage.read<std::uint32_t>(kojo::endian::little);
        }
    }

    size_t size() {
        return entry_count * 31 + 16;
    }

    kojo::binary& write() {
        storage.clear();
        version = 1000;
        entry_count = entries.size();
        storage.write<std::uint32_t>(version, kojo::endian::little);
        storage.write<std::uint32_t>(entry_count, kojo::endian::little);
        storage.write<std::uint64_t>(first_pointer, kojo::endian::little);

        size_t i{0};
        for (auto& entry : entries) {
            entry.character_id_ptr = 24 * entry_count + i * 7 - i++ * 24;
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
};

    } // namespace ASBR
} // namespace nucc

#endif // KOJO_NUCC_ASBR_PLAYERCOLORPARAM