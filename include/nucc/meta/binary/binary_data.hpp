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

    template<std::integral T> void write_offset_str(std::string& str) {
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