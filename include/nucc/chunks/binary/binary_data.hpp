#ifndef KOJO_NUCC_BINARY_DATA
#define KOJO_NUCC_BINARY_DATA

#include "../../external/binary-lib/binary/binary.hpp"
#include "../../error_handling.hpp"
#include <nlohmann/json.hpp>

#include <format>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace nucc {

struct RGB {
    std::uint32_t red, green, blue, rgb;

    void consolidate() {
        rgb = (blue | ((green | (red << 8)) << 8)) << 8;
    }

    RGB hex_to_rgb(std::string hex_str) {
        std::erase(hex_str, '#');
        std::stringstream buffer;
        buffer << std::hex << hex_str.substr(0, 2);
        buffer >> red;
        buffer.clear();
        buffer << std::hex << hex_str.substr(2, 2);
        buffer >> green;
        buffer.clear();
        buffer << std::hex << hex_str.substr(4, 2);
        buffer >> blue;
        this->consolidate();
        return *this;
    }

    std::string rgb_to_hex() {
        return std::format("#{:02x}{:02x}{:02x}", red, green, blue);
    }
};

class Binary_Data {
public:


    // Return size of would-be binary data.
    virtual size_t size() { return -1; }
    // Clear all variables.
    virtual void clear() {}

    // Write to binary or JSON output.
    virtual std::uint64_t* write_to_bin() { return (std::uint64_t*)storage.data(); }
    virtual nlohmann::ordered_json write_to_json() { return nullptr; }

protected:
    kojo::binary storage;

    std::vector<std::string> str_tracker;
    std::uint64_t ptr_buffer64;
    size_t last_pos;

    unsigned int ceiling(int value, unsigned int factor) {
        return value + (factor - value % factor) % factor;
    }

    void write_offset_str(std::string& str) {
        if (str.size() > 0) {
            str_tracker.push_back(str);
            ptr_buffer64 -= (storage.get_pos() - last_pos);
            last_pos = storage.get_pos();
            storage.write<std::uint64_t>(ptr_buffer64, kojo::endian::little);
            ptr_buffer64 += ceiling(str.size() + 1, 8); // ceiling to nearest 8
        } else {
            storage.write<std::uint64_t>(0, kojo::endian::little);
        }
    }
};

} // namespace nucc

#endif // KOJO_NUCC_BINARY_DATA