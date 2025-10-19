#ifndef KOJO_NUCC_CHUNK_META_HPP
#define KOJO_NUCC_CHUNK_META_HPP

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