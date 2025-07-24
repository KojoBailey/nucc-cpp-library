#ifndef KOJO_NUCC_ASBR_PLAYERCOLORPARAM
#define KOJO_NUCC_ASBR_PLAYERCOLORPARAM

#include <nucc/chunks/binary/binary_data_new.hpp>
#include <nucc/chunks/binary/json_serializable.hpp>

#include <nucc/error_handler.hpp>
#include <nucc/rgb.hpp>

namespace nucc {
    namespace asbr {
        
class player_color_param : public binary_data {
public:
    struct entry {
        std::string character_id; // Length always 6.
        std::uint32_t tint_index; // Not written.
        std::uint32_t costume_index;
        rgb color;

        std::string key();
    };
    std::map<std::string, entry> entries;

    std::unordered_map<std::string, size_t> tint_tracker;

    int version() const { return VERSION; }

    size_t size() const override;

    void read(const std::byte* src, const size_t start = 0) override;
    const std::shared_ptr<std::vector<std::byte>> write() const override;

private:
    const std::uint32_t VERSION{1000};

    const size_t HEADER_SIZE{16};
    const size_t ENTRY_SIZE{24};
    const size_t CHARACTER_ID_LENGTH{8};
};

    } // namespace ASBR

template<>
class json_serializer<nucc::asbr::player_color_param> {
public:
    static nucc::asbr::player_color_param read(const nlohmann::ordered_json& json) {
        nucc::asbr::player_color_param param;

        for (const auto& [key, value] : json.items()) {
            if (key == "Version" || key == "Filetype") continue;

            // if (!value.is_string()) error::print(
            //     nucc::status_code::json_value,
            //     std::format("JSON data for entry \"{}\" is not a valid hex code.", key),
            //     "Ensure all hex codes are strings with the format \"#RRGGBB\"."
            // );
            // if (value.template get<std::string>().length() != 7) error::print(
            //     nucc::status_code::json_value,
            //     std::format("JSON data for entry \"{}\" is not a valid hex code.", key),
            //     "Ensure all hex codes are strings with the format \"#RRGGBB\". Alpha channel is not supported."
            // );

            nucc::asbr::player_color_param::entry entry_buffer;
            entry_buffer.character_id = key.substr(0, 4) + "0" + key.at(5);
            entry_buffer.costume_index = key.at(4) - '0';
            entry_buffer.color.from_hex_code(value);
            entry_buffer.tint_index = key.at(9);

            param.entries[key] = entry_buffer;
        }

        return param;
    }
    static nlohmann::ordered_json write(const nucc::asbr::player_color_param& param) {
        nlohmann::ordered_json json;

        json["Version"] = 250718;
        json["Filetype"] = "PlayerColorParam";

        for (auto& [key, entry] : param.entries) {
            json[key] = entry.color.to_hex_code();
        }

        return json;
    }
};

} // namespace nucc

#endif // KOJO_NUCC_ASBR_PLAYERCOLORPARAM