#ifndef KOJO_NUCC_ASBR_SPEAKINGLINEPARAM
#define KOJO_NUCC_ASBR_SPEAKINGLINEPARAM

#include "../binary_data.hpp"
#include "utils.hpp"

namespace nucc {
    namespace ASBR {
        
class SpeakingLineParam : public Binary_Data {
public:
    struct Entry {
        enum class Interaction_Type {
            UNKNOWN = 0,
            BATTLE_START = 1,
            ROUND_WIN = 2,
            BATTLE_WIN = 3
        };

        struct Character {
            std::string id;
            std::string dialogue;
        };

        Interaction_Type interaction_type;
        bool is_round_win{false};
        Character character_1;
        Character character_2;

        std::string key() {
            return std::format("{}-{}-{}", character_1.id, character_2.id, (int)interaction_type);
        }
    };

    std::uint32_t version;
    std::uint32_t entry_count;
    std::uint64_t first_pointer;
    std::map<std::string, Entry> entries;

    SpeakingLineParam(void* input, size_t size_input = -1) {
        load(input, size_input);
    }

    int load(void* input, size_t size_input = -1) {
        if (input == nullptr) return error_handler({
            nucc::Status_Code::POINTER_NULL,
            "Attempted to load SpeakingLineParam chunk data, but received null input.",
            "Ensure the provided data is being passed correctly."
        });
        storage.load(input, 0, size_input);

        version = storage.read<std::uint32_t>(kojo::endian::little);
        entry_count = storage.read<std::uint32_t>(kojo::endian::little);
        first_pointer = storage.read<std::uint64_t>(kojo::endian::little);
        storage.change_pos(first_pointer - 8);

        Entry entry_buffer;
        for (int i = 0; i < entry_count; i++) {
            entry_buffer.interaction_type       = (Entry::Interaction_Type)storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.is_round_win           = storage.read<std::uint32_t>(kojo::endian::little);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.character_1.id         = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.character_2.id         = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.character_1.dialogue   = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.character_2.dialogue   = storage.read<std::string>(0, ptr_buffer64 - 8);

            entries[entry_buffer.key()] = entry_buffer;
        }

        return 0;
    }
    int load(nlohmann::ordered_json input) {
        if (input.is_null()) return 0;

        for (const auto& [key, value] : input.items()) {
            Entry entry_buffer;

            if (value.contains("Interaction Type")) {
                if (value["Interaction Type"] == "Battle Start") {
                    entry_buffer.interaction_type = Entry::Interaction_Type::BATTLE_START;
                } else if (value["Interaction Type"] == "Round Win") {
                    entry_buffer.interaction_type = Entry::Interaction_Type::ROUND_WIN;
                } else if (value["Interaction Type"] == "Battle Win") {
                    entry_buffer.interaction_type = Entry::Interaction_Type::BATTLE_WIN;
                } else {
                    return error_handler({
                        nucc::Status_Code::JSON_VALUE,
                        std::format("In entry \"{}\", \"{}\" is not a recognised value for the \"Interaction Type\" field.", key, value["Interaction Type"].template get<std::string>()),
                        "Choose either \"Battle Start\", \"Round Win\", or \"Battle Win\"."
                    });
                }
            } else {
                return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain necessary field \"Interaction Type\".", key),
                    "Add the \"Interaction Type\" field."
                });
            }
            entry_buffer.is_round_win = (entry_buffer.interaction_type == Entry::Interaction_Type::ROUND_WIN);

            if (value.contains("Character 1")) {
                if (value["Character 1"].contains("Character"))
                    entry_buffer.character_1.id = get_character_id_ref(value["Character 1"]["Character"]);
                    else return error_handler({
                        nucc::Status_Code::JSON_MISSING_FIELD,
                        std::format("JSON data for entry \"{}\" does not contain necessary field \"Character\" within object \"Character 1\".", key),
                        "Add the \"Character\" field with the name or ID of a playable character."
                    });
                if(value["Character 1"].contains("Dialogue"))
                    entry_buffer.character_1.dialogue = value["Character 1"]["Dialogue"];
                    else return error_handler({
                        nucc::Status_Code::JSON_MISSING_FIELD,
                        std::format("JSON data for entry \"{}\" does not contain necessary field \"Dialogue\" within object \"Character 1\".", key),
                        "Add the \"Dialogue\" field with the ID of a voiceline."
                    });
            } else return error_handler({
                nucc::Status_Code::JSON_MISSING_FIELD,
                std::format("JSON data for entry \"{}\" does not contain necessary field \"Character 1\".", key),
                "Add the \"Character 1\" JSON object with its contents included as well."
            });

            if (value.contains("Character 2")) {
                if (value["Character 2"].contains("Character"))
                    entry_buffer.character_2.id = get_character_id_ref(value["Character 2"]["Character"]);
                    else return error_handler({
                        nucc::Status_Code::JSON_MISSING_FIELD,
                        std::format("JSON data for entry \"{}\" does not contain necessary field \"Character\" within object \"Character 2\".", key),
                        "Add the \"Character\" field with the name or ID of a playable character."
                    });
                if(value["Character 2"].contains("Dialogue"))
                    entry_buffer.character_2.dialogue = value["Character 2"]["Dialogue"];
                    else return error_handler({
                        nucc::Status_Code::JSON_MISSING_FIELD,
                        std::format("JSON data for entry \"{}\" does not contain necessary field \"Dialogue\" within object \"Character 2\".", key),
                        "Add the \"Dialogue\" field with the ID of a voiceline."
                    });
            } else return error_handler({
                nucc::Status_Code::JSON_MISSING_FIELD,
                std::format("JSON data for entry \"{}\" does not contain necessary field \"Character 2\".", key),
                "Add the \"Character 2\" JSON object with its contents included as well."
            });

            entries[entry_buffer.key()] = entry_buffer;
        }

        return 0;
    }

    size_t size() {
        size_t size_buffer = entries.size() * 40 + 8 + first_pointer;

        for (auto& [key, entry] : entries) {
            size_buffer += entry.character_1.id.size() + 1;
            size_buffer += entry.character_2.id.size() + 1;
            size_buffer += entry.character_1.dialogue.size() + 1;
            size_buffer += entry.character_2.dialogue.size() + 1;
        }

        return size_buffer;
    }
    void clear() {
        version = 1000;
        entry_count = 0;
        first_pointer = 8;
        entries.clear();
    }

    std::uint64_t* write_to_bin() {
        storage.clear();
        
        version = 1000;
        entry_count = entries.size();
        storage.write<std::uint32_t>(version, kojo::endian::little);
        storage.write<std::uint32_t>(entry_count, kojo::endian::little);
        storage.write<std::uint64_t>(first_pointer, kojo::endian::little);

        last_pos = 8 + first_pointer; // Size of header
        ptr_buffer64 = (40 * entry_count);
        for (auto& [key, entry] : entries) {
            storage.write<std::uint32_t>((int)entry.interaction_type, kojo::endian::little);
            storage.write<std::uint32_t>(entry.is_round_win, kojo::endian::little);
            write_offset_str(entry.character_1.id);
            write_offset_str(entry.character_2.id);
            write_offset_str(entry.character_1.dialogue);
            write_offset_str(entry.character_2.dialogue);
        }
        for (auto& [key, entry] : entries) {
            storage.write<std::string>(entry.character_1.id);
            storage.align_by(8);
            storage.write<std::string>(entry.character_2.id);
            storage.align_by(8);
            storage.write<std::string>(entry.character_1.dialogue);
            storage.align_by(8);
            storage.write<std::string>(entry.character_2.dialogue);
            storage.align_by(8);
        }

        return (std::uint64_t*)storage.data();
    }
    std::string write_to_json() {
        nlohmann::ordered_json json;

        json["Version"] = 241001;
        json["Filetype"] = "SpeakingLineParam";
        
        for (auto& [key, value] : entries) {
            std::string type_str;
            if (value.interaction_type == Entry::Interaction_Type::BATTLE_START) {
                type_str = "Battle Start";
            } else if (value.interaction_type == Entry::Interaction_Type::ROUND_WIN) {
                type_str = "Round Win";
            } else if (value.interaction_type == Entry::Interaction_Type::BATTLE_WIN) {
                type_str = "Battle Win";
            }

            auto& entry = json[std::format("{} vs {} - {}", value.character_1.id, value.character_2.id, type_str)];

            entry["Interaction Type"] = type_str;
            entry["Character 1"]["ID"] = value.character_1.id;
            entry["Character 1"]["Dialogue"] = value.character_1.dialogue;
            entry["Character 2"]["ID"] = value.character_2.id;
            entry["Character 2"]["Dialogue"] = value.character_2.dialogue;
        }

        return json.dump(2);
    }
};

    } // namespace ASBR
} // namespace nucc

#endif // KOJO_NUCC_ASBR_SPEAKINGLINEPARAM