#ifndef KOJO_NUCC_CHUNK_META
#define KOJO_NUCC_CHUNK_META

#include <kojo/binary.hpp>

namespace nucc {

class chunk_meta {
public:
    chunk_meta() = default;
    virtual ~chunk_meta() = default;

    virtual size_t load(kojo::binary_view input) = 0;
    virtual void update(kojo::binary_view data) = 0;
};

}

#endif // KOJO_NUCC_CHUNK_META