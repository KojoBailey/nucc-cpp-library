#ifndef KOJO_NUCC_ASBR_SPEAKINGLINEPARAM
#define KOJO_NUCC_ASBR_SPEAKINGLINEPARAM

#include "../binary_data.hpp"

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
            std::string audio{""};
        };

        Interaction_Type interaction_type;
        bool is_animation{false};
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

    static const std::string path() {
        return R"(SpeakingLineParam.bin)";
    }

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
            entry_buffer.is_animation           = storage.read<std::uint32_t>(kojo::endian::little);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.character_1.id         = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.character_2.id         = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.character_1.audio   = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.character_2.audio   = storage.read<std::string>(0, ptr_buffer64 - 8);

            entries[entry_buffer.key()] = entry_buffer;
        }

        return 0;
    }
    int load(nlohmann::ordered_json& input) {
        if (input.is_null()) return 0;

        for (const auto& [key, value] : input.items()) {
            if (key == "Version" || key == "Filetype") continue;
            
            Entry entry_buffer;
            entry_buffer.character_1.id = key.substr(0, 6);
            entry_buffer.character_2.id = key.substr(7, 6);
            std::string buffer = key.substr(14, key.size() - 14);
            if (buffer == "btlst")
                entry_buffer.interaction_type = Entry::Interaction_Type::BATTLE_START;
            else if (buffer == "btlwin")
                entry_buffer.interaction_type = Entry::Interaction_Type::BATTLE_WIN;
            else if (buffer == "rndwin") {
                entry_buffer.interaction_type = Entry::Interaction_Type::ROUND_WIN;
            } else return error_handler({
                nucc::Status_Code::JSON_VALUE,
                std::format("Key \"{}\" contains an unrecognised interaction type.", key),
                "Ensure this is either \"btlst\", \"btlwin\" or \"rndwin\"."
            });
            if (entries.contains(entry_buffer.key()))
                entry_buffer = entries[entry_buffer.key()];

            if (value.contains("Is_Animation"))
                entry_buffer.is_animation = value["Is_Animation"];
            if (value.contains("Character_1_Audio"))
                entry_buffer.character_1.audio = value["Character_1_Audio"];
            if (value.contains("Character_2_Audio"))
                entry_buffer.character_2.audio = value["Character_2_Audio"];

            entries[entry_buffer.key()] = entry_buffer;
        }

        return 0;
    }

    size_t size() {
        size_t size_buffer = entries.size() * 40 + 8 + first_pointer;

        for (auto& [key, entry] : entries) {
            size_buffer += entry.character_1.id.size() + 1;
            size_buffer += entry.character_2.id.size() + 1;
            size_buffer += entry.character_1.audio.size() + 1;
            size_buffer += entry.character_2.audio.size() + 1;
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
            storage.write<std::uint32_t>((int)entry.is_animation, kojo::endian::little);
            write_offset_str(entry.character_1.id);
            write_offset_str(entry.character_2.id);
            write_offset_str(entry.character_1.audio);
            write_offset_str(entry.character_2.audio);
        }
        for (auto& [key, entry] : entries) {
            storage.write<std::string>(entry.character_1.id);
            storage.align_by(8);
            storage.write<std::string>(entry.character_2.id);
            storage.align_by(8);
            storage.write<std::string>(entry.character_1.audio);
            storage.align_by(8);
            storage.write<std::string>(entry.character_2.audio);
            storage.align_by(8);
        }

        return (std::uint64_t*)storage.data();
    }
    nlohmann::ordered_json write_to_json(std::filesystem::path hashlist_path = "NO PATH") {
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
            entry["Character 1"]["Dialogue"] = value.character_1.audio;
            entry["Character 2"]["ID"] = value.character_2.id;
            entry["Character 2"]["Dialogue"] = value.character_2.audio;
        }

        return json;
    }
};

    } // namespace ASBR
} // namespace nucc

#endif // KOJO_NUCC_ASBR_SPEAKINGLINEPARAM