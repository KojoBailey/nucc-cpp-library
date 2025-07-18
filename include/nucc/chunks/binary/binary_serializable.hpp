#ifndef KOJO_NUCC_BINARY_SERIALIZABLE
#define KOJO_NUCC_BINARY_SERIALIZABLE

#include <kojo/binary.hpp>

namespace nucc {

class binary_serializable {
public:
    virtual ~binary_serializable() = default;

    binary_serializable(const std::byte* src, const size_t size = 0) {
        read_binary(src, size);
    }

    virtual void read_binary(const std::byte* src, const size_t size = 0) = 0;
    virtual std::shared_ptr<std::byte> write_binary() = 0;
};

}

#endif // KOJO_NUCC_BINARY_SERIALIZABLE