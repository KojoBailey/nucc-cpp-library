#ifndef KOJO_NUCC_ASBR_UTILS
#define KOJO_NUCC_ASBR_UTILS

#include <algorithm>
#include <string>
#include <unordered_map>


namespace nucc {
    namespace ASBR {

extern std::unordered_map<std::string, std::string> character_id_reflist;

std::string get_character_id_ref(std::string ref);

std::string lowercase_str(std::string input);

    } // ASBR
} // nucc

#endif // KOJO_NUCC_ASBR_UTILS