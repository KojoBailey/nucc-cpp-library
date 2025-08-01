#ifndef KOJO_NUCC_RGB
#define KOJO_NUCC_RGB

#include <cstdint>
#include <string>

namespace nucc {

class rgb {
public:
    std::uint32_t red{0}, green{0}, blue{0}, alpha{255};

    [[nodiscard]] std::uint32_t consolidate() const;

    rgb from_hex_code(std::string hex_code);
    [[nodiscard]] std::string to_hex_code() const;
};

}

#endif