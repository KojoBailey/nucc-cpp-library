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
    std::map<std::string, entry> entries;

    std::unordered_map<std::string, size_t> tint_tracker;

    int version() const { return VERSION; }

    size_t size() const override;

    void read_binary(const std::byte* src, const size_t start = 0) override;
    void read_json(const nlohmann::ordered_json& input) override;

    const std::shared_ptr<std::vector<std::byte>> write_binary() const override;
    nlohmann::ordered_json write_json() override;

private:
    const std::uint32_t VERSION{1000};

    const size_t HEADER_SIZE{16};
    const size_t ENTRY_SIZE{24};
    const size_t CHARACTER_ID_LENGTH{8};
};

    } // namespace ASBR
} // namespace nucc

#endif // KOJO_NUCC_ASBR_PLAYERCOLORPARAM