#ifndef KOJO_NUCC_ASBR_PLAYERCOLORPARAM
#define KOJO_NUCC_ASBR_PLAYERCOLORPARAM

#include <nucc/chunks/binary/binary_serializable.hpp>
#include <nucc/chunks/binary/json_serializable.hpp>

#include <nucc/rgb.hpp>

namespace nucc {
    namespace asbr {
        
class player_color_param : public binary_serializable, json_serializable {
public:
    struct entry {
        std::string character_id; // Length always 6.
        std::uint32_t tint_index; // Not written.
        std::uint32_t costume_index;
        rgb color;

        std::string key();
    };

    std::uint32_t version;
    std::uint32_t entry_count;
    std::uint64_t first_pointer;
    std::map<std::string, entry> entries;

    static constexpr std::string_view path() { return "PlayerColorParam.bin"; }

    void read_binary(const std::byte* src, const size_t size = 0) override;
    void read_json(const nlohmann::ordered_json& input) override;

    size_t size() {
        return entry_count * 31 + 8 + first_pointer;
    }
    void clear() {
        version = 1000;
        entry_count = 0;
        first_pointer = 8;
        entries.clear();
        tint_tracker.clear();
    }

    std::uint64_t* write_to_bin() {
        version = 1000;
        entry_count = entries.size();
        storage.write_int<std::uint32_t>(version, std::endian::little);
        storage.write_int<std::uint32_t>(entry_count, std::endian::little);
        storage.write_int<std::uint64_t>(first_pointer, std::endian::little);

        size_t i = 0;
        for (auto& [key, entry] : entries) {
            ptr_buffer64 = (24 * entry_count) + (i * 7) - (i++ * 2);
            storage.write_int<std::uint64_t>(ptr_buffer64, std::endian::little);
            storage.write_int<std::uint32_t>(entry.costume_index, std::endian::little);
            storage.write_int<std::uint32_t>(entry.color.red, std::endian::little);
            storage.write_int<std::uint32_t>(entry.color.green, std::endian::little);
            storage.write_int<std::uint32_t>(entry.color.blue, std::endian::little);
        }
        for (auto& [key, entry] : entries) {
            storage.write_str(entry.character_id);
        }

        return (std::uint64_t*)storage.data();
    }
    nlohmann::ordered_json write_to_json() {
        nlohmann::ordered_json json;

        json["Version"] = 241001;
        json["Filetype"] = "PlayerColorParam";
        
        for (auto& [key, entry] : entries) {
            json[key] = entry.color.rgb_to_hex();
        }

        return json;
    }

    std::unordered_map<std::string, int> tint_tracker;
};

    } // namespace ASBR
} // namespace nucc

#endif // KOJO_NUCC_ASBR_PLAYERCOLORPARAM