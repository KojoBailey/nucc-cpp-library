#ifndef KOJO_NUCC_ASBR_UTILS
#define KOJO_NUCC_ASBR_UTILS

#include <unordered_map>
#include <string>

namespace nucc {
    namespace ASBR {

extern std::unordered_map<std::string, std::string> character_id_reflist;

std::string get_character_id_ref(std::string ref);

    } // ASBR
} // nucc

#endif // KOJO_NUCC_ASBR_UTILS