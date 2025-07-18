#ifndef KOJO_NUCC_BINARY_SERIALIZABLE
#define KOJO_NUCC_BINARY_SERIALIZABLE

#include <kojo/binary.hpp>

namespace nucc {

class binary_serializable {
public:
    virtual ~binary_serializable() = default;

    binary_serializable(const std::byte* src, const size_t start = 0) {
        read_binary(src, start);
    }

    virtual size_t size() const = 0;

    virtual void read_binary(const std::byte* src, const size_t start = 0) = 0;
    virtual const std::shared_ptr<std::vector<std::byte>> write_binary() const = 0;
};

}

#endif // KOJO_NUCC_BINARY_SERIALIZABLE