#ifndef KOJO_NUCC_ASBR_MESSAGEINFO
#define KOJO_NUCC_ASBR_MESSAGEINFO

#include "../binary_data.hpp"
#include "utils.hpp"

namespace nucc {
    namespace ASBR {
        
class messageInfo : public Binary_Data {
public:
    struct Entry {
        std::uint32_t crc32_id;
        std::string message;
        std::uint32_t ref_crc32_id{0};
        std::int16_t is_ref{-1};
        std::int64_t char_index{-1};
        std::int64_t cue_id{-1};

        std::uint32_t key() {
            return crc32_id;
        }
    };

    std::uint32_t version{241003};
    std::uint32_t entry_count;
    std::uint64_t first_pointer;
    std::map<std::uint32_t, Entry> entries;

    messageInfo(void* input, size_t size_input = -1) {
        load(input, size_input);
    }

    int load(void* input, size_t size_input = -1) {
        if (input == nullptr) return error_handler({
            nucc::Status_Code::POINTER_NULL,
            "Attempted to load messageInfo chunk data, but received null input.",
            "Ensure the provided data is being passed correctly."
        });
        storage.load(input, 0, size_input);

        version = storage.read<std::uint32_t>(kojo::endian::little);
        entry_count = storage.read<std::uint32_t>(kojo::endian::little);
        first_pointer = storage.read<std::uint64_t>(kojo::endian::little);
        storage.change_pos(first_pointer - 8);

        Entry entry_buffer;
        for (int i = 0; i < entry_count; i++) {
            entry_buffer.crc32_id       = storage.read<std::uint32_t>(kojo::endian::big);
            storage.change_pos(12); // Skip unknown constants.
            ptr_buffer64                = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.message        = storage.read<std::string>(0, ptr_buffer64 - 8);
            entry_buffer.ref_crc32_id   = storage.read<std::uint32_t>(kojo::endian::big);
            entry_buffer.is_ref         = storage.read<std::int16_t>(kojo::endian::big);
            entry_buffer.char_index     = storage.read<std::int16_t>(kojo::endian::big);
            entry_buffer.cue_id         = storage.read<std::int16_t>(kojo::endian::big);
            storage.change_pos(6); // Skip unknown constants.

            entries[entry_buffer.key()] = entry_buffer;
        }

        return 0;
    }
    int load(nlohmann::ordered_json& input) {
        if (input.is_null()) return 0;

        for (const auto& [key, value] : input.items()) {
            Entry entry_buffer;

            entry_buffer.crc32_id = std::stoi(key, nullptr, 16);

            if (value.contains("Message")) {
                entry_buffer.message = value["Message"];
            } else {
                return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain necessary field \"Message\".", key),
                    "Add the \"Message\" field."
                });
            }
            
            if (value.contains("Reference Hash")) {
                entry_buffer.is_ref = 1;
                entry_buffer.ref_crc32_id = std::stoi(value["Reference Hash"].template get<std::string>(), nullptr, 16);
            }

            if (value.contains("Character"))
                entry_buffer.char_index = get_character_index_ref(value["Character"]);

            if (value.contains("ADX2 Cue Index"))
                entry_buffer.cue_id = value["ADX2 Cue Index"];

            entries[entry_buffer.key()] = entry_buffer;
        }

        return 0;
    }

    size_t size() {
        size_t size_buffer = entries.size() * 40 + 8 + first_pointer;
        size_t size_buffer2;

        for (auto& [key, entry] : entries) {
            if ((size_buffer2 = entry.message.size()) > 0)
                size_buffer += ceiling(size_buffer2 + 1, 8);
        }

        return size_buffer;
    }
    void clear() {
        entry_count = 0;
        first_pointer = 8;
        entries.clear();
    }

    std::uint64_t* write_to_bin() {
        storage.clear();
        
        entry_count = entries.size();
        storage.write<std::uint32_t>(version, kojo::endian::little);
        storage.write<std::uint32_t>(entry_count, kojo::endian::little);
        storage.write<std::uint64_t>(first_pointer, kojo::endian::little);

        last_pos = 8 + first_pointer; // Size of header
        ptr_buffer64 = (40 * entry_count);
        for (auto& [key, entry] : entries) {
            storage.write<std::uint32_t>(entry.crc32_id, kojo::endian::big);

            storage.write<std::uint32_t>(0, kojo::endian::little);
            storage.write<std::uint32_t>(0, kojo::endian::little);
            storage.write<std::uint32_t>(0, kojo::endian::little);

            write_offset_str(entry.message);
            storage.write<std::uint32_t>(entry.ref_crc32_id, kojo::endian::big);
            storage.write<std::int16_t>(entry.is_ref, kojo::endian::little);
            storage.write<std::int16_t>(entry.char_index, kojo::endian::little);
            storage.write<std::int16_t>(entry.cue_id, kojo::endian::little);

            storage.write<std::int16_t>(-1, kojo::endian::little);
            storage.write<std::uint32_t>(0, kojo::endian::little);
        }
        for (auto& str : str_tracker) {
            storage.write<std::string>(str);
            storage.align_by(8);
        }

        return (std::uint64_t*)storage.data();
    }
    std::string write_to_json() {
        nlohmann::ordered_json json;

        json["Version"] = 241003;
        json["Filetype"] = "messageInfo";
        
        for (auto& [key, value] : entries) {
            auto& entry = json[std::format("{:08x}", key)];
            entry["Message"] = value.message;

            if (value.is_ref == 1)
                entry["Reference Hash"] = std::format("{:08x}", value.ref_crc32_id);

            if (value.char_index != -1)
                entry["Character"] = convert_character_index(value.char_index);

            if (value.cue_id != -1)
                entry["ADX2 Cue ID"] = value.cue_id;
        }

        return json.dump(2);
    }
};

    } // namespace ASBR
} // namespace nucc

#endif // KOJO_NUCC_ASBR_MESSAGEINFO