#include <string>
#include <vector>
#include <cstdint>

struct NUT_Texture {
    std::uint32_t total_size, data_size;
    std::uint16_t header_size;
    std::uint8_t minimap_count;
    std::uint8_t pixel_format;
    std::uint16_t width, height;
    const std::string magic = "GIDX";
    std::uint32_t hash_id;
    std::vector<char> data;
};

struct NUT {
    const std::string magic = "NTP3";
    std::uint16_t version;
    std::uint16_t count;
    std::vector<NUT_Texture> textures;
};