#ifndef KOJO_NUCC_EOHPS3_UTILS
#define KOJO_NUCC_EOHPS3_UTILS

#include <string>
#include <unordered_map>

namespace nucc {
    namespace EOHPS3 {

extern std::unordered_map<std::string, std::string> character_id_reflist;
extern std::unordered_map<std::string, int> character_index_reflist;

std::string get_character_id_ref(std::string ref);

int get_character_index_ref(std::string ref);
std::string convert_character_index(int index);

    } // EOHPS3
} // nucc

#endif // KOJO_NUCC_EOHPS3_UTILS