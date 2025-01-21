#ifndef KOJO_NUCC_ASB_MESSAGEINFO
#define KOJO_NUCC_ASB_MESSAGEINFO

#include "../binary_data.hpp"

namespace nucc {
    namespace ASB {
        
class messageInfo : public Binary_Data {
public:
    struct Entry {
        std::uint32_t crc32_id;
        std::string message{"<EMPTY>"};
        std::int16_t file_index{-1};
        std::int16_t cue_index{-1};

        std::uint32_t key() {
            return crc32_id;
        }
        std::string order_key () {
            return std::format("{:03}{:03}{:08x}", file_index, cue_index, crc32_id);
        }
    };

    std::uint32_t version{250103};
    std::string language{"Unknown"};
    const int entry_size = 16;
    std::uint32_t entry_count;
    std::uint32_t first_pointer;
    std::map<std::uint32_t, Entry> entries;
    std::map<std::string, std::uint32_t> entry_order;

    messageInfo() {};
    messageInfo(void* input, size_t size_input = -1, std::string lang_input = "") {
        load(input, size_input, lang_input);
    }

    int load(void* input, size_t size_input = -1, std::string lang_input = "") {
        if (input == nullptr) return error_handler({
            nucc::Status_Code::POINTER_NULL,
            "Attempted to load messageInfo chunk data, but received null input.",
            "Ensure the provided data is being passed correctly."
        });
        storage.load(input, 0, size_input);

        version = storage.read<std::uint32_t>(std::endian::big);
        if (version != 1001)
            return error_handler({
                nucc::Status_Code::VERSION,
                std::format("Expected version `{}` for messageInfo data, but instead got `{}`.", 1001, version),
                std::format("Ensure the data is of version `{}`.", 1001)
            });
        entry_count = storage.read<std::uint32_t>(std::endian::big);
        first_pointer = storage.read<std::uint32_t>(std::endian::big);
        storage.change_pos(first_pointer - sizeof(first_pointer));

        Entry entry_buffer;
        for (int i = 0; i < entry_count; i++) {
            entry_buffer.crc32_id       = storage.read<std::uint32_t>(std::endian::big);
            storage.change_pos(4); // Skip unknown constant.
            ptr_buffer32                = storage.read<std::uint32_t>(std::endian::big);
            entry_buffer.message        = storage.read<std::string>(0, ptr_buffer32 - sizeof(ptr_buffer32));
            entry_buffer.file_index     = storage.read<std::int16_t>(std::endian::big);
            entry_buffer.cue_index      = storage.read<std::int16_t>(std::endian::big);

            entries[entry_buffer.key()] = entry_buffer;
            entry_order[entry_buffer.order_key()] = entry_buffer.key();
        }

        if (lang_input != "") language = lang_input;

        return 0;
    }
    int load(nlohmann::ordered_json& input) {
        if (input.is_null()) return 0;

        language = input["Language"];
        load_adx2_file_list();

        std::unordered_map<std::string, std::string> colors;
        for (auto& [key, value] : input["Colors"].items()) {
            colors[key] = value.template get<std::string>().substr(1, 8);
        }

        for (const auto& [key, value] : input.items()) {
            if (key == "Language" || key == "Version" || key == "Filetype" || key == "Colors") continue;

            Entry entry_buffer;

            if (std::regex_match(key, std::regex("^([0-9a-fA-F]{8})$"))) {
                entry_buffer.crc32_id = std::stoul(key, nullptr, 16);
            } else {
                entry_buffer.crc32_id = nucc::hash(key);
                if (kojo::system_endian() == std::endian::big)
                    entry_buffer.crc32_id = kojo::byteswap(entry_buffer.crc32_id);
            }

            if (entries.contains(entry_buffer.key()))
                entry_buffer = entries[entry_buffer.key()];

            if (value.contains("Message")) {
                if (value["Message"] == "<EMPTY>")
                    return error_handler({
                        nucc::Status_Code::JSON_VALUE,
                        std::format("Field \"Page\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                        "Change this value to something else."
                    });
                else {
                    entry_buffer.message = value["Message"];
                    for (auto& [key, value] : colors) {
                        entry_buffer.message = std::regex_replace(entry_buffer.message,
                            std::regex(std::format("<{}>", key)),
                            std::format("<color 0x{}>", value)
                        );
                        entry_buffer.message = std::regex_replace(entry_buffer.message,
                            std::regex(std::format("</{}>", key)),
                            "</color>"
                        );
                    }
                }
            } else if (entry_buffer.message == "<EMPTY>") return error_handler({
                nucc::Status_Code::JSON_MISSING_FIELD,
                std::format("JSON data for entry \"{}\" does not contain required field \"Message\".", key),
                "Add the \"Message\" field."
            });

            if (value.contains("ADX2_File"))
                entry_buffer.file_index = convert_file_str(value["ADX2_File"]);

            if (value.contains("ADX2_Cue_Index"))
                entry_buffer.cue_index = value["ADX2_Cue_Index"];

            entries[entry_buffer.key()] = entry_buffer;
        }

        return 0;
    }

    size_t size() {
        size_t size_buffer = entries.size() * entry_size + 8 + first_pointer;
        size_t size_buffer2;

        for (auto& [key, entry] : entries) {
            if ((size_buffer2 = entry.message.size()) > 0)
                size_buffer += ceiling(size_buffer2 + 1, 4);
        }

        return size_buffer;
    }
    void clear() {
        entry_count = 0;
        first_pointer = sizeof(first_pointer);
        entries.clear();
    }

    std::uint64_t* write_to_bin() {
        storage.clear();
        
        entry_count = entries.size();
        storage.write<std::uint32_t>(version, std::endian::big);
        storage.write<std::uint32_t>(entry_count, std::endian::big);
        storage.write<std::uint32_t>(first_pointer, std::endian::big);

        last_pos = 8 + first_pointer; // Size of header
        ptr_buffer32 = (entry_size * entry_count);
        for (auto& [key, entry] : entries) {
            storage.write<std::uint32_t>(entry.crc32_id, std::endian::big);

            storage.write<std::uint32_t>(0, std::endian::big); // unk

            write_offset_str32(entry.message);
            storage.write<std::int16_t>(entry.file_index, std::endian::big);
            storage.write<std::int16_t>(entry.cue_index, std::endian::big);
        }
        for (auto& str : str_tracker) {
            storage.write<std::string>(str);
            storage.align_by(4);
        }

        return (std::uint64_t*)storage.data();
    }
    nlohmann::ordered_json write_to_json(std::string hashlist_path) {
        std::unordered_map<std::string, std::string> hashlist;
        kojo::binary hashlist_data{hashlist_path};
        while (!hashlist_data.at_end()) {
            std::string buffer = hashlist_data.read<std::string>();
            hashlist[buffer] = hashlist_data.read<std::string>();
        }
        nlohmann::ordered_json json;

        json["Version"] = 250103;
        json["Filetype"] = "messageInfo";
        json["Language"] = language;
        
        for (auto& [key, value] : entry_order) {
            auto& entry = entries[value];
            if (kojo::system_endian() != std::endian::big)
                value = kojo::byteswap(value);

            std::string hash = std::format("{:08x}", value);
            if (hashlist.contains(hash)) {
                hash = hashlist[hash];
            }
            nlohmann::ordered_json& json_entry = json[hash];

            json_entry["Message"] = entry.message;

            if (entry.file_index != -1)
                json_entry["ADX2_File"] = convert_file_index(entry.file_index);

            if (entry.cue_index != -1)
                json_entry["ADX2_Cue_Index"] = entry.cue_index;
        }

        return json;
    }

private:
    std::unordered_map<std::string, int> adx2_file_list;

    void load_adx2_file_list() {
        adx2_file_list["v_sys_etc"]     = 1;
        adx2_file_list["v_mob"]         = 2;
        adx2_file_list["v_btl_0bao01"]  = 3;
        adx2_file_list["v_btl_1dio01"]  = 4;
        adx2_file_list["v_btl_1jnt01"]  = 5;
        adx2_file_list["v_btl_2csr01"]  = 7;
        adx2_file_list["v_btl_2esd01"]  = 8;
        adx2_file_list["v_btl_2jsp01"]  = 9;
        adx2_file_list["v_btl_2krs01"]  = 10;
        adx2_file_list["v_btl_2wmu01"]  = 12;
        adx2_file_list["v_btl_3abd01"]  = 13;
        adx2_file_list["v_btl_3dio01"]  = 14;
        adx2_file_list["v_btl_3hhs01"]  = 15;
        adx2_file_list["v_btl_3jtr01"]  = 18;
        adx2_file_list["v_btl_3kki01"]  = 19;
        adx2_file_list["v_btl_3pln01"]  = 20;
        adx2_file_list["v_btl_4jsk01"]  = 22;
        adx2_file_list["v_btl_4koi01"]  = 24;
        adx2_file_list["v_btl_4kir01"]  = 25;
        adx2_file_list["v_btl_4oky01"]  = 26;
        adx2_file_list["v_btl_4oti01"]  = 27;
        adx2_file_list["v_btl_4rhn01"]  = 28;
        adx2_file_list["v_btl_5bct01"]  = 30;
        adx2_file_list["v_btl_5dvl01"]  = 31;
        adx2_file_list["v_btl_5grn01"]  = 33;
        adx2_file_list["v_btl_5nrc01"]  = 35;
        adx2_file_list["v_btl_6elm01"]  = 37;
        adx2_file_list["v_btl_6pci01"]  = 39;
        adx2_file_list["v_btl_7jir01"]  = 40;
        adx2_file_list["v_btl_7jny01"]  = 41;
        adx2_file_list["v_ev_main"]     = 44;
        adx2_file_list["v_card_0"]      = 56;
        adx2_file_list["v_card_1"]      = 57;
        adx2_file_list["v_etc"]         = 58;
    }
    std::string convert_file_index(int index) {
        switch(index) {
            case 1:     return "v_sys_etc";
            case 2:     return "v_mob";
            case 4:     return "v_btl_1dio01";
            case 5:     return "v_btl_1jnt01";
            case 7:     return "v_btl_2csr01";
            case 8:     return "v_btl_2esd01";
            case 9:     return "v_btl_2jsp01";
            case 10:    return "v_btl_2krs01";
            case 12:    return "v_btl_2wmu01";
            case 13:    return "v_btl_3abd01";
            case 14:    return "v_btl_3dio01";
            case 15:    return "v_btl_3hhs01";
            case 18:    return "v_btl_3jtr01";
            case 19:    return "v_btl_3kki01";
            case 20:    return "v_btl_3pln01";
            case 22:    return "v_btl_4jsk01";
            case 24:    return "v_btl_4koi01";
            case 25:    return "v_btl_4kir01";
            case 26:    return "v_btl_4oky01";
            case 27:    return "v_btl_4oti01";
            case 28:    return "v_btl_4rhn01";
            case 30:    return "v_btl_5bct01";
            case 31:    return "v_btl_5dvl01";
            case 33:    return "v_btl_5grn01";
            case 35:    return "v_btl_5nrc01";
            case 37:    return "v_btl_6elm01";
            case 39:    return "v_btl_6pci01";
            case 40:    return "v_btl_7jir01";
            case 41:    return "v_btl_7jny01";
            case 44:    return "v_ev_main";
            case 56:    return "v_card_0";
            case 57:    return "v_card_1";
            case 58:    return "v_etc";
        }
        return std::to_string(index);
    }
    int convert_file_str(std::string file) {
        if (!adx2_file_list.contains(file)) return -1;
        return adx2_file_list[file];
    }
};

    } // namespace ASB
} // namespace nucc

#endif // KOJO_NUCC_ASB_MESSAGEINFO