#ifndef KOJO_NUCC_CHUNK_NULL
#define KOJO_NUCC_CHUNK_NULL

#include <nucc/chunks/chunk_meta.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class chunk_null : public chunk_meta {
public:
    size_t load(kojo::binary_view input) override { return 0; }
};

}

#endif // KOJO_NUCC_CHUNK_NULL