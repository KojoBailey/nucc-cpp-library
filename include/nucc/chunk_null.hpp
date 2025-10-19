#ifndef KOJO_NUCC_CHUNK_NULL_HPP
#define KOJO_NUCC_CHUNK_NULL_HPP

#include <nucc/chunk_meta.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class chunk_null : public chunk_meta {
public:
        chunk_null() = default;
        size_t load(kojo::binary_view input) override { return 0; }
        void update(kojo::binary_view) override {}
};

}

#endif