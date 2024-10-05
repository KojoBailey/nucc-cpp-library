#ifndef KOJO_NUCC_ASBR_UTILS
#define KOJO_NUCC_ASBR_UTILS

#include <algorithm>
#include <string>
#include <unordered_map>


namespace nucc {
    namespace ASBR {

extern std::unordered_map<std::string, std::string> character_id_reflist;
extern std::unordered_map<std::string, int> character_index_reflist;

std::string get_character_id_ref(std::string ref);

int get_character_index_ref(std::string ref);
std::string convert_character_index(int index);

std::string lowercase_str(std::string input);

    } // ASBR
} // nucc

#endif // KOJO_NUCC_ASBR_UTILS