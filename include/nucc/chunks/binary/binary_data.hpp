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

struct rgb {
    std::uint32_t red, green, blue, alpha;

    std::uint32_t consolidate() {
        return alpha | ((blue | ((green | (red << 8)) << 8)) << 8);
    }

    rgb hex_to_rgb(std::string hex_str) {
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

struct crc32 {
    std::uint32_t id;

    crc32() = default;
    crc32(std::uint32_t _id) : id(_id) {}
    crc32(std::string str) {
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
            kojo::binary::set_endian(id, std::endian::big);
        }
    }

    std::string to_string() {
        return std::format("{:08x}", id);
    }
};

class binary_data {
public:
    virtual size_t size() const = 0;
    virtual void clear() = 0;

    virtual void read_bin(const std::byte* src, const size_t size = 0) = 0;
    virtual void read_json(const nlohmann::ordered_json& input) = 0;

    virtual std::shared_ptr<std::byte> write_bin() = 0;
    virtual nlohmann::ordered_json write_json() = 0;

protected:
    kojo::binary storage;

    std::vector<std::string> str_tracker;
    std::uint32_t ptr_buffer32;
    std::uint64_t ptr_buffer64;
    size_t last_pos;

    unsigned int ceiling(int value, unsigned int factor) {
        return value + (factor - value % factor) % factor;
    }

    template<std::integral T>void write_offset_str(std::string& str) {
        if (str.size() > 0) {
            str_tracker.push_back(str);
            ptr_buffer64 -= (storage.get_pos() - last_pos);
            last_pos = storage.get_pos();
            storage.write<T>(static_cast<T>(ptr_buffer64), std::endian::little);
            ptr_buffer64 += ceiling(str.size() + 1, sizeof(T)); // ceiling to nearest 8
        } else {
            storage.write<T>(0, std::endian::little);
        }
    }
};

} // namespace nucc

#endif // KOJO_NUCC_BINARY_DATA