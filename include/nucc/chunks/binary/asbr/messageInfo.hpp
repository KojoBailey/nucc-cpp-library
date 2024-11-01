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
        std::int64_t file_index{-1};
        std::int64_t cue_index{-1};

        std::uint32_t key() {
            return crc32_id;
        }
        std::string order_key () {
            return std::format("{:03}{:03}{:08x}", file_index, cue_index, crc32_id);
        }
    };

    std::uint32_t version{241003};
    std::uint32_t entry_count;
    std::uint64_t first_pointer;
    std::map<std::uint32_t, Entry> entries;
    std::map<std::string, std::uint32_t> entry_order;

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
        if (version != 1001)
            return error_handler({
                nucc::Status_Code::VERSION,
                std::format("Expected version `{}` for messageInfo data, but instead got `{}`.", 1001, version),
                std::format("Ensure the data is of version `{}`.", 1001)
            });
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
            entry_buffer.is_ref         = storage.read<std::int16_t>(kojo::endian::little);
            entry_buffer.file_index     = storage.read<std::int16_t>(kojo::endian::little);
            entry_buffer.cue_index         = storage.read<std::int16_t>(kojo::endian::little);
            storage.change_pos(6); // Skip unknown constants.

            entries[entry_buffer.key()] = entry_buffer;
            entry_order[entry_buffer.order_key()] = entry_buffer.key();
        }

        return 0;
    }
    int load(nlohmann::ordered_json input) {
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
                entry_buffer.file_index = get_character_index_ref(value["Character"]);

            if (value.contains("ADX2 Cue Index"))
                entry_buffer.cue_index = value["ADX2 Cue Index"];

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
            storage.write<std::int16_t>(entry.file_index, kojo::endian::little);
            storage.write<std::int16_t>(entry.cue_index, kojo::endian::little);

            storage.write<std::int16_t>(-1, kojo::endian::little);
            storage.write<std::uint32_t>(0, kojo::endian::little);
        }
        for (auto& str : str_tracker) {
            storage.write<std::string>(str);
            storage.align_by(8);
        }

        return (std::uint64_t*)storage.data();
    }
    nlohmann::ordered_json write_to_json() {
        nlohmann::ordered_json json;

        json["Version"] = 241003;
        json["Filetype"] = "messageInfo";
        
        for (auto& [key, value] : entry_order) {
            auto& entry = entries[value];
            auto& json_entry = json[std::format("{:08x}", value)];
            json_entry["Message"] = entry.message;

            if (entry.is_ref == 1)
                json_entry["Reference Hash"] = std::format("{:08x}", entry.ref_crc32_id);

            if (entry.file_index != -1)
                json_entry["ADX2 File"] = convert_file_index(entry.file_index);

            if (entry.cue_index != -1)
                json_entry["ADX2 Cue Index"] = entry.cue_index;
        }

        return json;
    }

private:
    std::string convert_file_index(int index) {
        switch(index) {
            case 1:     return "v_sys_etc";
            case 2:     return "v_mob";
            case 3:     return "v_btl_0bao01";
            case 4:     return "v_btl_1dio01";
            case 5:     return "v_btl_1jnt01";
            case 6:     return "v_btl_0bao01";
            case 7:     return "v_btl_1zpl01";
            case 8:     return "v_btl_2csr01";
            case 9:     return "v_btl_2esd01";
            case 10:    return "v_btl_2jsp01";
            case 11:    return "v_btl_2krs01";
            case 12:    return "v_btl_2lsa01";
            case 13:    return "v_btl_2wmu01";
            case 14:    return "v_btl_3abd01";
            case 15:    return "v_btl_3dio01";
            case 16:    return "v_btl_3hhs01";
            case 17:    return "v_btl_3igy01";
            case 18:    return "v_btl_3jsp01";
            case 19:    return "v_btl_3jtr01";
            case 20:    return "v_btl_3kki01";
            case 21:    return "v_btl_3mra01";
            case 22:    return "v_btl_3pln01";
            case 23:    return "v_btl_3psp01";
            case 24:    return "v_btl_3vni01";
            case 25:    return "v_btl_4jsk01";
            case 26:    return "v_btl_4jtr01";
            case 27:    return "v_btl_4kir01";
            case 28:    return "v_btl_4koi01";
            case 29:    return "v_btl_4kwk01";
            case 30:    return "v_btl_4oky01";
            case 31:    return "v_btl_4oti01";
            case 32:    return "v_btl_4rhn01";
            case 33:    return "v_btl_4sgc01";
            case 34:    return "v_btl_4ykk01";
            case 35:    return "v_btl_5bct01";
            case 36:    return "v_btl_5dvl01";
            case 37:    return "v_btl_5fgo01";
            case 38:    return "v_btl_5gac01";
            case 39:    return "v_btl_5grn01";
            case 40:    return "v_btl_5mst01";
            case 41:    return "v_btl_5nrc01";
            case 42:    return "v_btl_5prs01";
            case 43:    return "v_btl_5trs01";
            case 44:    return "v_btl_6ans01";
            case 45:    return "v_btl_6elm01";
            case 46:    return "v_btl_6fit01";
            case 47:    return "v_btl_6jln01";
            case 48:    return "v_btl_6pci02";
            case 49:    return "v_btl_7dio01";
            case 50:    return "v_btl_7jir01";
            case 51:    return "v_btl_7jny01";
            case 52:    return "v_btl_7vtn01";
            case 53:    return "v_btl_8jsk01";
            case 56:    return "v_card";
            case 57:    return "v_gallery";
            case 58:    return "v_btl_6wet01";
            case 59:    return "v_sys_6wet01";
            case 60:    return "v_btl_5ris01";
            case 61:    return "v_sys_5ris01";
            case 62:    return "v_btl_6pci01";
            case 63:    return "v_sys_6pci01";
            case 64:    return "v_btl_2shm01";
            case 65:    return "v_sys_2shm01";
            case 66:    return "v_btl_4kch01";
            case 67:    return "v_sys_4kch01";
            case 68:    return "v_btl_7dio02";
            case 69:    return "v_sys_7dio02";
            case 70:    return "v_btl_5abc01";
            case 71:    return "v_sys_5abc01";
            case 72:    return "v_btl_4fgm01";
            case 73:    return "v_sys_4fgm01";
            case 74:    return "v_btl_8wou01";
            case 75:    return "v_sys_8wou01";
        }
        return std::to_string(index);
    }
};

    } // namespace ASBR
} // namespace nucc

#endif // KOJO_NUCC_ASBR_MESSAGEINFO