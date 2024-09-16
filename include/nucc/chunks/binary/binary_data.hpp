#ifndef KOJO_NUCC_BINARY_DATA
#define KOJO_NUCC_BINARY_DATA

#include "../../external/binary-lib/binary/binary.hpp"
#include <nlohmann/json.hpp>

#include <format>

namespace nucc {

class Binary_Data {
public:
    // Load data into variables (different for each dervied class).
    // Do not overwrite existing data, instead merging over.
    virtual void load(void* input, size_t size_input = -1) {}
    virtual void load(nlohmann::ordered_json input) {}

    // Return size of would-be binary data.
    virtual size_t size() { return -1; }
    // Clear all variables.
    virtual void clear() {}

    // Write to binary or JSON output.
    virtual kojo::binary& write_to_bin() { return storage; }
    virtual std::string write_to_json() { return 0; }

protected:
    kojo::binary storage;
};

} // namespace nucc

#endif // KOJO_NUCC_BINARY_DATA