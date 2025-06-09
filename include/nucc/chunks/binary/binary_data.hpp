#ifndef KOJO_NUCC_BINARY_DATA
#define KOJO_NUCC_BINARY_DATA

#include "../../error_handling.hpp"
#include "../../utils.hpp"
#include "../../hash.hpp"

#include <kojo/binary.hpp>
#include <nlohmann/json.hpp>

#include <bit>
#include <format>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace nucc {

struct RGB {
    std::uint32_t red, green, blue, alpha;

    std::uint32_t consolidate() {
        return alpha | ((blue | ((green | (red << 8)) << 8)) << 8);
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

struct CRC32 {
    std::uint32_t id;

    CRC32() = default;
    CRC32(std::uint32_t _id) : id(_id) {}
    CRC32(std::string str) {
        load(str);
    }

    void load(std::uint32_t _id) {
        id = _id;
    }
    void load(std::string str) {
        if (std::regex_match(str, std::regex("^([0-9a-fA-F]{8})$"))) {
            id = std::stoul(str, nullptr, 16);
        } else {
            id = nucc::hash(str);
            if (kojo::system_endian() == std::endian::big)
                id = kojo::byteswap(id);
        }
    }

    std::string to_string() {
        return std::format("{:08x}", id);
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
    std::uint32_t ptr_buffer32;
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
            storage.write_int<std::uint64_t>(ptr_buffer64, std::endian::little);
            ptr_buffer64 += ceiling(str.size() + 1, 8); // ceiling to nearest 8
        } else {
            storage.write_int<std::uint64_t>(0, std::endian::little);
        }
    }
    void write_offset_str32(std::string& str) {
        if (str.size() > 0) {
            str_tracker.push_back(str);
            ptr_buffer32 -= (storage.get_pos() - last_pos);
            last_pos = storage.get_pos();
            storage.write_int<std::uint32_t>(ptr_buffer32, std::endian::little);
            ptr_buffer32 += ceiling(str.size() + 1, 4); // ceiling to nearest 4
        } else {
            storage.write_int<std::uint32_t>(0, std::endian::little);
        }
    }
};

} // namespace nucc

#endif // KOJO_NUCC_BINARY_DATA