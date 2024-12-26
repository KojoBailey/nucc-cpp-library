#ifndef KOJO_NUCC_ASBR
#define KOJO_NUCC_ASBR

#include "asbr/messageInfo.hpp"
#include "asbr/PlayerColorParam.hpp"
#include "asbr/SpeakingLineParam.hpp"
#include "asbr/MainModeParam.hpp"

#include <cstdint>

namespace nucc::ASBR::cache {

struct vector {
    std::uint64_t* unk00;
    std::uint64_t* unk08;
    std::uint64_t* unk10;
    char* start;
    char* position;
    char* end;
};

struct hash_string {
    std::uint32_t hash;
    std::uint32_t active;
    const char* string;
};

struct PlayerColorParam {
    std::uint32_t character_id_hash;
    std::int32_t costume_index;
    std::uint64_t padding;
    struct {
        float red, green, blue, alpha;
    } color;
};

}

#endif // KOJO_NUCC_ASBR