#ifndef KOJO_NUCC_BYTES_SERIALIZABLE
#define KOJO_NUCC_BYTES_SERIALIZABLE

#include <kojo/binary.hpp>

namespace nucc {

class bytes_serializable {
public:
    virtual ~bytes_serializable() = default;

    bytes_serializable(const std::byte* src, const size_t size = 0) {
        read_bytes(src, size);
    }

    virtual void read_bytes(const std::byte* src, const size_t size = 0) = 0;
    virtual std::shared_ptr<std::byte> write_bytes() = 0;
};

}

#endif // KOJO_NUCC_BYTES_SERIALIZABLE