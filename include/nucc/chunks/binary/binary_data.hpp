#ifndef KOJO_NUCC_BINARY_DATA
#define KOJO_NUCC_BINARY_DATA

#include "../../external/binary-lib/binary/binary.hpp"

#include <vector>
#include <cstdint>

namespace nucc {

class Binary_Data {
public:
    virtual size_t size() { return 0; }
    virtual kojo::binary& write() { return storage; }

protected:
    kojo::binary storage;
};

} // namespace nucc

#endif // KOJO_NUCC_BINARY_DATA