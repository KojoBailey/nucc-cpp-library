#ifndef KOJO_NUCC_ASBR_MAINMODEPARAM
#define KOJO_NUCC_ASBR_MAINMODEPARAM

#include "../binary_data.hpp"
#include "utils.hpp"

namespace nucc {
    namespace ASBR {
        
class MainModeParam : public Binary_Data {
public:
    struct Entry {
        enum class Panel_Type {
            NORMAL = 0,
            EXTRA = 1,
            BOSS = 2
        };

        std::uint32_t index;
        std::uint64_t part{(std::uint64_t)-1};
        std::string panel_id; // key
        std::uint64_t page; // auto-calculated
        std::string boss_panel{"<EMPTY>"}; // auto-calculated

        struct {
            std::string up{""};
            std::string down{""};
            std::string left{""};
            std::string right{""};
        } adjacent_panels;

        Panel_Type type{Panel_Type::NORMAL};
        std::uint32_t display_difficulty;
        std::uint32_t cpu_level{(std::uint32_t)-1};
        std::uint32_t gold_reward{0};
        std::string stage_id{"<EMPTY>"};

        struct Character{
            std::string id{"<EMPTY>"};
            std::string assist_id{""};
            std::string btlst_id{"<EMPTY>"};
            std::string btlwin_id{"<EMPTY>"};
        };
        Character player, enemy;
        std::uint64_t first_speaker{0};

        std::int32_t special_rule[4]{0, -1, -1, -1};

        struct Secret_Mission {
            std::int32_t condition{-1};
            std::int32_t unk{0}; // need to figure this one out
            std::string reward_id{""};
            std::uint64_t gold_reward{0};
        };
        Secret_Mission secret_mission[4]; // 4th doesn't work in ASBR

        std::uint32_t unk0{2};
        std::uint32_t unk1{0};
        std::uint32_t unk2{2};

        std::string key() {
            return panel_id;
        }
    };

    std::uint32_t version;
    std::uint32_t entry_count;
    std::uint64_t first_pointer;
    std::map<std::string, Entry> entries;

    static const std::string path() {
        return R"(MainModeParam.bin)";
    }

    MainModeParam(void* input, size_t size_input = -1) {
        load(input, size_input);
    }

    int load(void* input, size_t size_input = -1) {
        if (input == nullptr) return error_handler({
            nucc::Status_Code::POINTER_NULL,
            "Attempted to load MainModeParam chunk data, but received null input.",
            "Ensure the provided data is being passed correctly."
        });
        storage.load(input, 0, size_input);

        version = storage.read<std::uint32_t>(kojo::endian::little);
        entry_count = storage.read<std::uint32_t>(kojo::endian::little);
        first_pointer = storage.read<std::uint64_t>(kojo::endian::little);
        storage.change_pos(first_pointer - 8);

        Entry entry_buffer;
        for (int i = 0; i < entry_count; i++) {
            entry_buffer.part                   = storage.read<std::uint64_t>(kojo::endian::little);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.panel_id               = storage.read<std::string>(0, ptr_buffer64 - 8);
            entry_buffer.page                   = storage.read<std::uint64_t>(kojo::endian::little);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.boss_panel             = storage.read<std::string>(0, ptr_buffer64 - 8);
            storage.change_pos(16); // 2 unk
            entry_buffer.gold_reward            = storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.type                   = (Entry::Panel_Type)storage.read<std::uint32_t>(kojo::endian::little);

            // Adjacent panels
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.adjacent_panels.up     = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.adjacent_panels.down   = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.adjacent_panels.left   = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.adjacent_panels.right  = storage.read<std::string>(0, ptr_buffer64 - 8);

            entry_buffer.display_difficulty     = storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.cpu_level              = storage.read<std::uint32_t>(kojo::endian::little);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.stage_id               = storage.read<std::string>(0, ptr_buffer64 - 8);
            storage.change_pos(8); // unk
            entry_buffer.first_speaker          = storage.read<std::uint64_t>(kojo::endian::little);

            // Player
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.player.id              = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.player.assist_id       = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.player.btlst_id        = storage.read<std::string>(0, ptr_buffer64 - 8);
            storage.change_pos(8); // unk
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.player.btlwin_id       = storage.read<std::string>(0, ptr_buffer64 - 8);

            // Enemy
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.enemy.id               = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.enemy.assist_id        = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.enemy.btlst_id         = storage.read<std::string>(0, ptr_buffer64 - 8);
            storage.change_pos(8); // unk
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.enemy.btlwin_id        = storage.read<std::string>(0, ptr_buffer64 - 8);

            // Special rules
            entry_buffer.special_rule[0]    = storage.read<std::int32_t>(kojo::endian::little);
            entry_buffer.special_rule[1]    = storage.read<std::int32_t>(kojo::endian::little);
            entry_buffer.special_rule[2]    = storage.read<std::int32_t>(kojo::endian::little);
            entry_buffer.special_rule[3]    = storage.read<std::int32_t>(kojo::endian::little);

            // Secret missions
            for (int i = 0; i < 4; i++) {
                entry_buffer.secret_mission[i].condition    = storage.read<std::int32_t>(kojo::endian::little);
                entry_buffer.secret_mission[i].unk          = storage.read<std::int32_t>(kojo::endian::little);
                ptr_buffer64                                = storage.read<std::uint64_t>(kojo::endian::little);
                entry_buffer.secret_mission[i].reward_id    = storage.read<std::string>(0, ptr_buffer64 - 8);
                entry_buffer.secret_mission[i].gold_reward  = storage.read<std::uint64_t>(kojo::endian::little);
            }

            entry_buffer.unk0                                   = storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.unk1                                   = storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.unk2                                   = storage.read<std::uint32_t>(kojo::endian::little);

            entry_buffer.index                                  = storage.read<std::uint32_t>(kojo::endian::little);

            entries[entry_buffer.key()] = entry_buffer;
        }
    
        return 0;
    }
    int load(nlohmann::ordered_json& input, bool merging = false) {
        if (input.is_null()) return 0;

        load_stage_reflist();

        for (const auto& [key, value] : input.items()) {
            if (key == "Version" || key == "Filetype") continue;
            
            Entry entry_buffer;
            entry_buffer.panel_id = key;
            if (entries.contains(key))
                entry_buffer = entries[key];

            if (value.contains("Part")) {
                if (value["Part"] == -1)
                    return error_handler({
                        nucc::Status_Code::JSON_VALUE,
                        std::format("Field \"Part\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                        "Change this value to something else."
                    });
                else entry_buffer.part = value["Part"];
            } else if (entry_buffer.part == -1) return error_handler({
                nucc::Status_Code::JSON_MISSING_FIELD,
                std::format("JSON data for entry \"{}\" does not contain required field \"Part\".", key),
                "Add the \"Part\" field depending on which Part the panel belongs to."
            });

            if (value.contains("Boss_Panel")) {
                if (value["Boss_Panel"] == "<EMPTY>")
                    return error_handler({
                        nucc::Status_Code::JSON_VALUE,
                        std::format("Field \"Boss_Panel\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                        "Change this value to something else."
                    });
                else entry_buffer.boss_panel = value["Boss_Panel"];
            } else if (entry_buffer.boss_panel == "<EMPTY>") {
                entry_buffer.boss_panel = entry_buffer.panel_id.substr(0, 9) + "08";
            }

            if (value.contains("Page")) {
                if (value["Page"] == -1)
                    return error_handler({
                        nucc::Status_Code::JSON_VALUE,
                        std::format("Field \"Page\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                        "Change this value to something else."
                    });
                else entry_buffer.page = value["Page"];
            } else if (entry_buffer.page == -1) {
                entry_buffer.page = std::stoi(entry_buffer.panel_id.substr(6, 2));
            }

            if (value.contains("Index")) {
                if (value["Index"] == -1)
                    return error_handler({
                        nucc::Status_Code::JSON_VALUE,
                        std::format("Field \"Index\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                        "Change this value to something else."
                    });
                else entry_buffer.index = value["Index"];
            } else if (entry_buffer.index == -1) {
                entry_buffer.index = entry_buffer.page * std::stoi(entry_buffer.panel_id.substr(9, 2));
            }

            if (value.contains("Adjacent Panels")) {
                auto& adjacent_panels = value["Adjacent Panels"];
                if (adjacent_panels.contains("Up"))
                    entry_buffer.adjacent_panels.up     = adjacent_panels["Up"];
                if (adjacent_panels.contains("Down"))
                    entry_buffer.adjacent_panels.down   = adjacent_panels["Down"];
                if (adjacent_panels.contains("Left"))
                    entry_buffer.adjacent_panels.left   = adjacent_panels["Left"];
                if (adjacent_panels.contains("Right"))
                    entry_buffer.adjacent_panels.right  = adjacent_panels["Right"];
            }

            if (value.contains("Type"))
                entry_buffer.type = str_to_Panel_Type(value["Type"]);

            if (value.contains("Stars")) {
                if (value["Stars"] == -1)
                    return error_handler({
                        nucc::Status_Code::JSON_VALUE,
                        std::format("Field \"Stars\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                        "Change this value to something else."
                    });
                else entry_buffer.display_difficulty = value["Stars"].template get<int>() - 1;
            } else if (entry_buffer.display_difficulty == -1) return error_handler({
                nucc::Status_Code::JSON_MISSING_FIELD,
                std::format("JSON data for entry \"{}\" does not contain required field \"Stars\".", key),
                "Add the \"Stars\" field with a value from 1-5."
            });

            if (value.contains("CPU_Level")) {
                if (value["CPU_Level"] == -1)
                    return error_handler({
                        nucc::Status_Code::JSON_VALUE,
                        std::format("Field \"CPU_Level\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                        "Change this value to something else."
                    });
                else entry_buffer.cpu_level = value["CPU_Level"].template get<int>() - 1;
            } else if (entry_buffer.cpu_level == -1) {
                entry_buffer.cpu_level = entry_buffer.display_difficulty;
            }

            if (value.contains("Gold_Reward")) 
                entry_buffer.gold_reward = value["Gold_Reward"];

            if (value.contains("Stage")) {
                if (value["Stage"] == "<EMPTY>")
                    return error_handler({
                        nucc::Status_Code::JSON_VALUE,
                        std::format("Field \"Stage\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                        "Change this value to something else."
                    });
                else entry_buffer.stage_id = get_stage_ref(value["Stage"]);
            } else if (entry_buffer.stage_id == "<EMPTY>") return error_handler({
                nucc::Status_Code::JSON_MISSING_FIELD,
                std::format("JSON data for entry \"{}\" does not contain required field \"Stage\".", key),
                "Add the \"Stage\" field with the stage for the battle to take place on."
            });

            if (value.contains("Player_Information")) {
                auto& player_information = value["Player_Information"];

                if (player_information.contains("Character")) {
                    if (player_information["Character"] == "<EMPTY>")
                        return error_handler({
                            nucc::Status_Code::JSON_VALUE,
                            std::format("Field \"Character\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                            "Change this value to something else."
                        });
                    else entry_buffer.player.id = get_character_id_ref(player_information["Character"]);
                } else if (entry_buffer.player.id == "<EMPTY>") return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain required field \"Character\" within object \"Player_Information\".", key),
                    "Add the \"Character\" field with the name or ID of a playable character."
                });

                if (player_information.contains("Assist"))
                    entry_buffer.player.assist_id = get_character_id_ref(player_information["Assist"]);

                if (player_information.contains("Start_Dialogue")) {
                    if (player_information["Start_Dialogue"] == "<EMPTY>")
                        return error_handler({
                            nucc::Status_Code::JSON_VALUE,
                            std::format("Field \"Start_Dialogue\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                            "Change this value to something else."
                        });
                    else entry_buffer.player.btlst_id = player_information["Start_Dialogue"];
                } else if (entry_buffer.player.btlst_id == "<EMPTY>") return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain required field \"Start_Dialogue\" within object \"Player_Information\".", key),
                    "Add the \"Start_Dialogue\" field with the ID of the desired battle start voiceline."
                });

                if (player_information.contains("Win_Dialogue")) {
                    if (player_information["Win_Dialogue"] == "<EMPTY>")
                        return error_handler({
                            nucc::Status_Code::JSON_VALUE,
                            std::format("Field \"Win_Dialogue\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                            "Change this value to something else."
                        });
                    else entry_buffer.player.btlwin_id = player_information["Win_Dialogue"];
                } else if (entry_buffer.player.btlwin_id == "<EMPTY>") return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain required field \"Win_Dialogue\" within object \"Player_Information\".", key),
                    "Add the \"Win_Dialogue\" field with the ID of the desired battle win voiceline."
                });
            } else if (!merging) {
                return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain required field \"Player_Information\".", key),
                    "Add the \"Player_Information\" object with its contents included as well."
                });
            }

            if (value.contains("Enemy_Information")) {
                auto& enemy_information = value["Enemy_Information"];

                if (enemy_information.contains("Character")) {
                    if (enemy_information["Character"] == "<EMPTY>")
                        return error_handler({
                            nucc::Status_Code::JSON_VALUE,
                            std::format("Field \"Character\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                            "Change this value to something else."
                        });
                    else entry_buffer.enemy.id = get_character_id_ref(enemy_information["Character"]);
                } else if (entry_buffer.enemy.id == "<EMPTY>") return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain required field \"Character\" within object \"Enemy_Information\".", key),
                    "Add the \"Character\" field with the name or ID of a playable character."
                });

                if (enemy_information.contains("Assist"))
                    entry_buffer.enemy.assist_id = get_character_id_ref(enemy_information["Assist"]);

                if (enemy_information.contains("Start_Dialogue")) {
                    if (enemy_information["Start_Dialogue"] == "<EMPTY>")
                        return error_handler({
                            nucc::Status_Code::JSON_VALUE,
                            std::format("Field \"Start_Dialogue\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                            "Change this value to something else."
                        });
                    else entry_buffer.enemy.btlst_id = enemy_information["Start_Dialogue"];
                } else if (entry_buffer.enemy.btlst_id == "<EMPTY>") return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain required field \"Start_Dialogue\" within object \"Enemy_Information\".", key),
                    "Add the \"Start_Dialogue\" field with the ID of the desired battle start voiceline."
                });

                if (enemy_information.contains("Win_Dialogue")) {
                    if (enemy_information["Win_Dialogue"] == "<EMPTY>")
                        return error_handler({
                            nucc::Status_Code::JSON_VALUE,
                            std::format("Field \"Win_Dialogue\" in JSON entry \"{}\" contains value reserved for error detection.", key),
                            "Change this value to something else."
                        });
                    else entry_buffer.enemy.btlwin_id = enemy_information["Win_Dialogue"];
                } else if (entry_buffer.enemy.btlwin_id == "<EMPTY>") return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain required field \"Win_Dialogue\" within object \"Enemy_Information\".", key),
                    "Add the \"Win_Dialogue\" field with the ID of the desired battle win voiceline."
                });
            } else if (!merging) {
                return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain required field \"Enemy_Information\".", key),
                    "Add the \"Enemy_Information\" object with its contents included as well."
                });
            }

            if (value.contains("First_Speaker")) {
                std::string first_speaker = lowercase_str(value["First_Speaker"]);
                if (first_speaker == "enemy") entry_buffer.first_speaker = 1;
                else if (first_speaker == "player") entry_buffer.first_speaker = 0;
                else return error_handler({
                    nucc::Status_Code::JSON_VALUE,
                    std::format("Field \"First_Speaker\" for entry \"{}\" is an invalid value.", key),
                    "Change the value to either \"Player\" or \"Enemy\" depending on who is to speak first."
                });
            }

            if (value.contains("Special_Rules")) {
                for (int i = 0; i < 4; i++) {
                    if (value["Special_Rules"].contains(std::format("Rule_{}", i + 1)))
                        entry_buffer.special_rule[i] = value["Special_Rules"][std::format("Rule_{}", i + 1)];
                }
            }

            if (value.contains("Secret_Missions")) {
                for (int i = 1; i <= 4; i++) {
                    std::string mission_title = std::format("Mission_{}", i);
                    if (value["Secret_Missions"].contains(mission_title)) {
                        auto& mission = value["Secret_Missions"][mission_title];
                        if (mission.contains("Condition"))
                            entry_buffer.secret_mission[i - 1].condition = mission["Condition"];
                        if (mission.contains("Reward"))
                            entry_buffer.secret_mission[i - 1].reward_id = mission["Reward"];
                        if (mission.contains("Gold_Reward"))
                            entry_buffer.secret_mission[i - 1].gold_reward = mission["Gold_Reward"];
                    }
                }
            }

            entries[entry_buffer.key()] = entry_buffer;
        }
    
        return 0;
    }

    size_t size() {
        size_t size_buffer = entries.size() * 328 + 8 + first_pointer;
        size_t size_buffer2;

        auto increase_size = [&](std::string& value) {
            if ((size_buffer2 = value.size()) > 0)
                size_buffer += ceiling(size_buffer2 + 1, 8);
        };

        for (auto& [key, entry] : entries) {
            increase_size(entry.panel_id);
            increase_size(entry.boss_panel);
            increase_size(entry.adjacent_panels.up);
            increase_size(entry.adjacent_panels.down);
            increase_size(entry.adjacent_panels.left);
            increase_size(entry.adjacent_panels.right);
            increase_size(entry.stage_id);
            increase_size(entry.player.id);
            increase_size(entry.player.assist_id);
            increase_size(entry.player.btlst_id);
            increase_size(entry.player.btlwin_id);
            increase_size(entry.enemy.id);
            increase_size(entry.enemy.assist_id);
            increase_size(entry.enemy.btlst_id);
            increase_size(entry.enemy.btlwin_id);
            increase_size(entry.secret_mission[0].reward_id);
            increase_size(entry.secret_mission[1].reward_id);
            increase_size(entry.secret_mission[2].reward_id);
            increase_size(entry.secret_mission[3].reward_id);
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

        str_tracker.clear();
        last_pos = 8 + first_pointer; // Size of header
        ptr_buffer64 = 328 * entry_count;
        for (auto& [key, entry] : entries) {
            storage.write<std::uint64_t>(entry.part, kojo::endian::little);
            write_offset_str(entry.panel_id);
            storage.write<std::uint64_t>(entry.page, kojo::endian::little);
            write_offset_str(entry.boss_panel);
            storage.write<std::uint64_t>(0, kojo::endian::little); // unk
            storage.write<std::uint64_t>(0, kojo::endian::little); // unk
            storage.write<std::uint32_t>(entry.gold_reward, kojo::endian::little);
            storage.write<std::uint32_t>((int)entry.type, kojo::endian::little);

            write_offset_str(entry.adjacent_panels.up);
            write_offset_str(entry.adjacent_panels.down);
            write_offset_str(entry.adjacent_panels.left);
            write_offset_str(entry.adjacent_panels.right);

            storage.write<std::uint32_t>(entry.display_difficulty, kojo::endian::little);
            storage.write<std::uint32_t>(entry.cpu_level, kojo::endian::little);
            write_offset_str(entry.stage_id);
            storage.write<std::uint64_t>(1, kojo::endian::little); // unk
            storage.write<std::uint64_t>(entry.first_speaker, kojo::endian::little);

            write_offset_str(entry.player.id);
            write_offset_str(entry.player.assist_id);
            write_offset_str(entry.player.btlst_id);
            storage.write<std::uint64_t>(0, kojo::endian::little); // unk
            write_offset_str(entry.player.btlwin_id);
            write_offset_str(entry.enemy.id);
            write_offset_str(entry.enemy.assist_id);
            write_offset_str(entry.enemy.btlst_id);
            storage.write<std::uint64_t>(0, kojo::endian::little); // unk
            write_offset_str(entry.enemy.btlwin_id);

            for (int i = 0; i < 4; i++) {
              storage.write<std::uint32_t>((int)entry.special_rule[i], kojo::endian::little);
            }

            for (int i = 0; i < 4; i++) {
                storage.write<std::uint32_t>((int)entry.secret_mission[i].condition, kojo::endian::little);
                storage.write<std::int32_t>(entry.secret_mission[i].unk, kojo::endian::little);
                write_offset_str(entry.secret_mission[i].reward_id);
                storage.write<std::uint64_t>(entry.secret_mission[i].gold_reward, kojo::endian::little);
            }

            storage.write<std::uint32_t>(entry.unk0, kojo::endian::little);
            storage.write<std::uint32_t>(entry.unk1, kojo::endian::little);
            storage.write<std::uint32_t>(entry.unk2, kojo::endian::little);

            storage.write<std::uint32_t>(entry.index, kojo::endian::little);
        }
        for (auto& str : str_tracker) {
            storage.write<std::string>(str);
            storage.align_by(8);
        }

        return (std::uint64_t*)storage.data();
    }
    nlohmann::ordered_json write_to_json(std::filesystem::path hashlist_path = "NO PATH") {
        nlohmann::ordered_json json;
        json["Version"] = 241001;
        json["Filetype"] = "MainModeParam";
        
        for (auto& [key, value] : entries) {
            auto& entry = json[value.panel_id];

            entry["Index"] = value.index;
            entry["Part"] = value.part;
            entry["Adjacent Panels"]["Up"] = value.adjacent_panels.up;
            entry["Adjacent Panels"]["Down"] = value.adjacent_panels.down;
            entry["Adjacent Panels"]["Left"] = value.adjacent_panels.left;
            entry["Adjacent Panels"]["Right"] = value.adjacent_panels.right;

            switch (value.type) {
                case Entry::Panel_Type::NORMAL  : entry["Type"] = "NORMAL"; break;
                case Entry::Panel_Type::EXTRA   : entry["Type"] = "EXTRA"; break;
                case Entry::Panel_Type::BOSS    : entry["Type"] = "BOSS"; break;
            }

            entry["Difficulty Stars"] = value.display_difficulty;
            entry["CPU Level"] = value.cpu_level;
            entry["Gold Reward"] = value.gold_reward;
            entry["Stage"] = value.stage_id;
        }

        return json;
    }

private:
    std::unordered_map<std::string, std::string> stage_reflist;

    Entry::Panel_Type str_to_Panel_Type(std::string str) {
        std::string low_str = lowercase_str(str);
        if (low_str == "extra") return Entry::Panel_Type::EXTRA;
        if (low_str == "boss") return Entry::Panel_Type::BOSS;
        return Entry::Panel_Type::NORMAL;
    }

    void load_stage_reflist() {
        stage_reflist["dio's castle"]                   = "STAGE_1_DIOCASTLE";

        stage_reflist["the battlefield"]                = "STAGE_2_COLOSSEO";
        stage_reflist["battlefield"]                    = "STAGE_2_COLOSSEO";

        stage_reflist["dio's mansion"]                  = "STAGE_3_DIOCASTLE";
        stage_reflist["cairo streets"]                  = "STAGE_3_CAIRO";
        stage_reflist["cairo overpass"]                 = "STAGE_3_CAIROCITY_BRIDGE";
        stage_reflist["cairo bridge"]                   = "STAGE_3_CAIROCITY_BRIDGE";

        stage_reflist["morioh town"]                    = "STAGE_4_MOT";
        stage_reflist["boyoyoing cape"]                 = "STAGE_4_BOYOYONH_CAPE";
        stage_reflist["kira estate"]                    = "STAGE_4_KIH";
        stage_reflist["kira house"]                     = "STAGE_4_KIH";
        stage_reflist["kira residence"]                 = "STAGE_4_KIH";

        stage_reflist["naples station"]                 = "STAGE_5_NEAPOLIS";
        stage_reflist["naples train station"]           = "STAGE_5_NEAPOLIS";
        stage_reflist["rome streets"]                   = "STAGE_5_ROMA";
        stage_reflist["colosseum"]                      = "STAGE_5_COLOSSEUM";

        stage_reflist["green dolphin street prison"]    = "STAGE_6_GD";
        stage_reflist["green dolphin"]                  = "STAGE_6_GD";
        stage_reflist["everglades"]                     = "STAGE_6_WETLANDS";
        stage_reflist["wetlands"]                       = "STAGE_6_WETLANDS";
        stage_reflist["kennedy space center"]           = "STAGE_6_KSC";
        stage_reflist["kennedy space centre"]           = "STAGE_6_KSC";

        stage_reflist["rocky mountains village"]        = "STAGE_7_ROCKIES_VILLAGE";
        stage_reflist["philadelphia coastline"]         = "STAGE_7_PSS";
        stage_reflist["philadelphia seaside"]           = "STAGE_7_PSS";

        stage_reflist["wall eyes"]                      = "STAGE_8_KBM";
    }
    std::string get_stage_ref(std::string ref) {
        if (!stage_reflist.contains(lowercase_str(ref))) return ref;
        return stage_reflist[lowercase_str(ref)];
    }
};

    } // namespace ASBR
} // namespace nucc

#endif // KOJO_NUCC_ASBR_MAINMODEPARAM