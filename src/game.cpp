#include <nucc/game.hpp>
#include <nucc/utils/utils.hpp>

#include <unordered_map>

namespace nucc {

std::string game_to_string(game game) {
    switch (game) {
        case game::nsuns3   : return "Naruto Shippuden: Ultimate Ninja Storm 3";
        case game::asb      : return "JoJo's Bizarre Adventure: All-Star Battle";
        case game::nsunsr   : return "Naruto Shippuden: Ultimate Ninja Storm Revolution";
        case game::eohps3   : return "JoJo's Bizarre Adventure: Eyes of Heaven (PS3)";
        case game::eohps4   : return "JoJo's Bizarre Adventure: Eyes of Heaven (PS4)";
        case game::nsuns4   : return "Naruto Shippuden: Ultimate Ninja Storm 4";
        case game::asbr     : return "JoJo's Bizarre Adventure: All-Star Battle R";
        case game::nxbunsc  : return "NARUTO X BORUTO Ultimate Ninja STORM CONNECTIONS";
    }
    return "Unknown";
}

game string_to_game(std::string str) {
    const std::unordered_map<std::string_view, game> string_game_map = {
        {"naruto shippuden: ultimate ninja storm 3", game::nsuns3},
        {"nsuns3", game::nsuns3},
        {"jojo's bizarre adventure: all-star battle", game::asb},
        {"asb", game::asb},
        {"naruto shippuden: ultimate ninja storm revolution", game::nsunsr},
        {"nsunsr", game::nsunsr},
        {"jojo's bizarre adventure: eyes of heaven (ps3)", game::eohps3},
        {"eohps3", game::eohps3},
        {"jojo's bizarre adventure: eyes of heaven (ps4)", game::eohps4},
        {"eohps4", game::eohps4},
        {"naruto shippuden: ultimate ninja storm 4", game::nsuns4},
        {"nsuns4", game::nsuns4},
        {"jojo's bizarre adventure: all-star battle r", game::asbr},
        {"asbr", game::asbr},
        {"naruto x boruto ultimate ninja storm connections", game::nxbunsc},
        {"nxbunsc", game::nxbunsc},
    };

    str = to_lowercase(str);
    auto iterator = string_game_map.find(str);
    if (iterator != string_game_map.end())
        return iterator->second;
    return game::unknown;
}

}