#ifndef KOJO_NUCC_EOHPS4_MESSAGEINFO
#define KOJO_NUCC_EOHPS4_MESSAGEINFO

#include "../binary_data.hpp"

namespace nucc {
    namespace EOHPS4 {
        
class messageInfo : public Binary_Data {
public:
    struct Entry {
        std::uint32_t crc32_id;
        std::string message{"<EMPTY>"};
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

    std::uint32_t version{250103};
    std::string language{"Unknown"};
    std::uint32_t entry_count;
    std::uint64_t first_pointer;
    std::map<std::uint32_t, Entry> entries;
    std::map<std::string, std::uint32_t> entry_order;

    static const std::string path() {
        return R"(PS4/.{3}/messageInfo\.bin)";
    }

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

        version = storage.read<std::uint32_t>(std::endian::little);
        if (version != 1001)
            return error_handler({
                nucc::Status_Code::VERSION,
                std::format("Expected version `{}` for messageInfo data, but instead got `{}`.", 1001, version),
                std::format("Ensure the data is of version `{}`.", 1001)
            });
        entry_count = storage.read<std::uint32_t>(std::endian::little);
        first_pointer = storage.read<std::uint64_t>(std::endian::little);
        storage.change_pos(first_pointer - 8);

        Entry entry_buffer;
        for (int i = 0; i < entry_count; i++) {
            entry_buffer.crc32_id       = storage.read<std::uint32_t>(std::endian::big);
            storage.change_pos(12); // Skip unknown constants.
            ptr_buffer64                = storage.read<std::uint64_t>(std::endian::little);
            entry_buffer.message        = storage.read<std::string>(0, ptr_buffer64 - 8);
            entry_buffer.ref_crc32_id   = storage.read<std::uint32_t>(std::endian::big);
            entry_buffer.is_ref         = storage.read<std::int16_t>(std::endian::little);
            entry_buffer.file_index     = storage.read<std::int16_t>(std::endian::little);
            entry_buffer.cue_index         = storage.read<std::int16_t>(std::endian::little);
            storage.change_pos(6); // Skip unknown constants.

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
            
            if (value.contains("Reference_Hash")) {
                entry_buffer.is_ref = 1;
                entry_buffer.ref_crc32_id = std::stoul(value["Reference_Hash"].template get<std::string>(), nullptr, 16);
            }

            if (value.contains("ADX2_File"))
                entry_buffer.file_index = convert_file_str(value["ADX2_File"]);

            if (value.contains("ADX2_Cue_Index"))
                entry_buffer.cue_index = value["ADX2_Cue_Index"];

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
        storage.write<std::uint32_t>(version, std::endian::little);
        storage.write<std::uint32_t>(entry_count, std::endian::little);
        storage.write<std::uint64_t>(first_pointer, std::endian::little);

        last_pos = 8 + first_pointer; // Size of header
        ptr_buffer64 = (40 * entry_count);
        for (auto& [key, entry] : entries) {
            storage.write<std::uint32_t>(entry.crc32_id, std::endian::big);

            storage.write<std::uint32_t>(0, std::endian::little);
            storage.write<std::uint32_t>(0, std::endian::little);
            storage.write<std::uint32_t>(0, std::endian::little);

            write_offset_str(entry.message);
            storage.write<std::uint32_t>(entry.ref_crc32_id, std::endian::big);
            storage.write<std::int16_t>(entry.is_ref, std::endian::little);
            storage.write<std::int16_t>(entry.file_index, std::endian::little);
            storage.write<std::int16_t>(entry.cue_index, std::endian::little);

            storage.write<std::int16_t>(-1, std::endian::little);
            storage.write<std::uint32_t>(0, std::endian::little);
        }
        for (auto& str : str_tracker) {
            storage.write<std::string>(str);
            storage.align_by(8);
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

            if (entry.is_ref == 1)
                json_entry["Reference_Hash"] = std::format("{:08x}", entry.ref_crc32_id);

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
        adx2_file_list["v_mob"]         = 2;
        adx2_file_list["v_btl_2csr01"]  = 6;
        adx2_file_list["v_btl_2jsp01"]  = 7;
        adx2_file_list["v_btl_2shm01"]  = 8;
        adx2_file_list["v_btl_3jtr01"]  = 9;
        adx2_file_list["v_btl_3kki01"]  = 10;
        adx2_file_list["v_btl_4jsk01"]  = 11;
        adx2_file_list["v_btl_5trs01"]  = 12;
        adx2_file_list["v_btl_7dio01"]  = 13;
        adx2_file_list["v_btl_1dio01"]  = 14;
        adx2_file_list["v_btl_1jnt01"]  = 15;
        adx2_file_list["v_btl_1sdw01"]  = 16;
        adx2_file_list["v_btl_1zpl01"]  = 17;
        adx2_file_list["v_btl_2esd01"]  = 18;
        adx2_file_list["v_btl_2krs01"]  = 19;
        adx2_file_list["v_btl_2lsa01"]  = 20;
        adx2_file_list["v_btl_2wmu01"]  = 21;
        adx2_file_list["v_btl_3abd01"]  = 22;
        adx2_file_list["v_btl_3dio01"]  = 23;
        adx2_file_list["v_btl_3hhs01"]  = 24;
        adx2_file_list["v_btl_3igy01"]  = 25;
        adx2_file_list["v_btl_3jsp01"]  = 26;
        adx2_file_list["v_btl_3pln01"]  = 27;
        adx2_file_list["v_btl_3psp01"]  = 28;
        adx2_file_list["v_btl_3vni01"]  = 29;
        adx2_file_list["v_btl_4kir01"]  = 30;
        adx2_file_list["v_btl_4koi01"]  = 31;
        adx2_file_list["v_btl_4kwk01"]  = 32;
        adx2_file_list["v_btl_4oky01"]  = 33;
        adx2_file_list["v_btl_4oti01"]  = 34;
        adx2_file_list["v_btl_4rhn01"]  = 35;
        adx2_file_list["v_btl_4sgc01"]  = 36;
        adx2_file_list["v_btl_4ykk01"]  = 37;
        adx2_file_list["v_btl_5bct01"]  = 38;
        adx2_file_list["v_btl_5dvl01"]  = 39;
        adx2_file_list["v_btl_5fgo01"]  = 40;
        adx2_file_list["v_btl_5grn01"]  = 41;
        adx2_file_list["v_btl_5mst01"]  = 42;
        adx2_file_list["v_btl_5nrc01"]  = 43;
        adx2_file_list["v_btl_6ans01"]  = 44;
        adx2_file_list["v_btl_6elm01"]  = 45;
        adx2_file_list["v_btl_6jln01"]  = 46;
        adx2_file_list["v_btl_6pci01"]  = 47;
        adx2_file_list["v_btl_6wet01"]  = 48;
        adx2_file_list["v_btl_7jir01"]  = 49;
        adx2_file_list["v_btl_7jny01"]  = 50;
        adx2_file_list["v_btl_7vtn01"]  = 51;
        adx2_file_list["v_btl_8jsk01"]  = 52;
        adx2_file_list["v_btl_3udr01"]  = 53;
        adx2_file_list["v_btl_3mra01"]  = 54;
        adx2_file_list["v_btl_8jsh01"]  = 55;
        adx2_file_list["v_btl_3jtr61"]  = 57;
        adx2_file_list["v_btl_3jtr71"]  = 58;
        adx2_file_list["v_btl_6wet11"]  = 59;
        adx2_file_list["v_main01"]      = 60;
        adx2_file_list["v_main02"]      = 61;
        adx2_file_list["v_main03"]      = 62;
        adx2_file_list["v_main04"]      = 63;
        adx2_file_list["v_main05"]      = 64;
        adx2_file_list["v_main06"]      = 65;
        adx2_file_list["v_main07"]      = 66;
        adx2_file_list["v_main08"]      = 67;
        adx2_file_list["v_main09"]      = 68;
        adx2_file_list["v_main10"]      = 69;
        adx2_file_list["v_main11"]      = 70;
        adx2_file_list["v_main12"]      = 71;
        adx2_file_list["v_card"]        = 72;
        adx2_file_list["v_gallery"]     = 73;
    }
    std::string convert_file_index(int index) {
        switch(index) {
            case 2:     return "v_mob";
            case 6:     return "v_btl_2csr01";
            case 7:     return "v_btl_2jsp01";
            case 8:     return "v_btl_2shm01";
            case 9:     return "v_btl_3jtr01";
            case 10:    return "v_btl_3kki01";
            case 11:    return "v_btl_4jsk01";
            case 12:    return "v_btl_5trs01";
            case 13:    return "v_btl_7dio01";
            case 14:    return "v_btl_1dio01";
            case 15:    return "v_btl_1jnt01";
            case 16:    return "v_btl_1sdw01";
            case 17:    return "v_btl_1zpl01";
            case 18:    return "v_btl_2esd01";
            case 19:    return "v_btl_2krs01";
            case 20:    return "v_btl_2lsa01";
            case 21:    return "v_btl_2wmu01";
            case 22:    return "v_btl_3abd01";
            case 23:    return "v_btl_3dio01";
            case 24:    return "v_btl_3hhs01";
            case 25:    return "v_btl_3igy01";
            case 26:    return "v_btl_3jsp01";
            case 27:    return "v_btl_3pln01";
            case 28:    return "v_btl_3psp01";
            case 29:    return "v_btl_3vni01";
            case 30:    return "v_btl_4kir01";
            case 31:    return "v_btl_4koi01";
            case 32:    return "v_btl_4kwk01";
            case 33:    return "v_btl_4oky01";
            case 34:    return "v_btl_4oti01";
            case 35:    return "v_btl_4rhn01";
            case 36:    return "v_btl_4sgc01";
            case 37:    return "v_btl_4ykk01";
            case 38:    return "v_btl_5bct01";
            case 39:    return "v_btl_5dvl01";
            case 40:    return "v_btl_5fgo01";
            case 41:    return "v_btl_5grn01";
            case 42:    return "v_btl_5mst01";
            case 43:    return "v_btl_5nrc01";
            case 44:    return "v_btl_6ans01";
            case 45:    return "v_btl_6elm01";
            case 46:    return "v_btl_6jln01";
            case 47:    return "v_btl_6pci01";
            case 48:    return "v_btl_6wet01";
            case 49:    return "v_btl_7jir01";
            case 50:    return "v_btl_7jny01";
            case 51:    return "v_btl_7vtn01";
            case 52:    return "v_btl_8jsk01";
            case 53:    return "v_btl_3udr01";
            case 54:    return "v_btl_3mra01";
            case 55:    return "v_btl_8jsh01";
            case 57:    return "v_btl_3jtr61";
            case 58:    return "v_btl_3jtr71";
            case 59:    return "v_btl_6wet11";
            case 60:    return "v_main01";
            case 61:    return "v_main02";
            case 62:    return "v_main03";
            case 63:    return "v_main04";
            case 64:    return "v_main05";
            case 65:    return "v_main06";
            case 66:    return "v_main07";
            case 67:    return "v_main08";
            case 68:    return "v_main09";
            case 69:    return "v_main10";
            case 70:    return "v_main11";
            case 71:    return "v_main12";
            case 72:    return "v_card";
            case 73:    return "v_gallery";
        }
        return std::to_string(index);
    }
    int convert_file_str(std::string file) {
        if (!adx2_file_list.contains(file)) return -1;
        return adx2_file_list[file];
    }
};

    } // namespace EOHPS4
} // namespace nucc

#endif // KOJO_NUCC_EOHPS4_MESSAGEINFO