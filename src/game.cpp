#include <nucc/game.hpp>
#include <nucc/utils.hpp>

#include <unordered_map>

using namespace nucc;

std::string game_to_string(Game game) {
    switch (game) {
        case Game::NSUNS3   : return "Naruto Shippuden: Ultimate Ninja Storm 3";
        case Game::ASB      : return "JoJo's Bizarre Adventure: All-Star Battle";
        case Game::NSUNSR   : return "Naruto Shippuden: Ultimate Ninja Storm Revolution";
        case Game::EOHPS3   : return "JoJo's Bizarre Adventure: Eyes of Heaven (PS3)";
        case Game::EOHPS4   : return "JoJo's Bizarre Adventure: Eyes of Heaven (PS4)";
        case Game::NSUNS4   : return "Naruto Shippuden: Ultimate Ninja Storm 4";
        case Game::ASBR     : return "JoJo's Bizarre Adventure: All-Star Battle R";
        case Game::NXBUNSC  : return "NARUTO X BORUTO Ultimate Ninja STORM CONNECTIONS";
    }
    return "Unknown";
}

Game string_to_game(std::string str) {
    static const std::unordered_map<std::string_view, Game> string_game_map = {
        {"naruto shippuden: ultimate ninja storm 3", Game::NSUNS3},
        {"nsuns3", Game::NSUNS3},
        {"jojo's bizarre adventure: all-star battle", Game::ASB},
        {"asb", Game::ASB},
        {"naruto shippuden: ultimate ninja storm revolution", Game::NSUNSR},
        {"nsunsr", Game::NSUNSR},
        {"jojo's bizarre adventure: eyes of heaven (ps3)", Game::EOHPS3},
        {"eohps3", Game::EOHPS3},
        {"jojo's bizarre adventure: eyes of heaven (ps4)", Game::EOHPS4},
        {"eohps4", Game::EOHPS4},
        {"naruto shippuden: ultimate ninja storm 4", Game::NSUNS4},
        {"nsuns4", Game::NSUNS4},
        {"jojo's bizarre adventure: all-star battle r", Game::ASBR},
        {"asbr", Game::ASBR},
        {"naruto x boruto ultimate ninja storm connections", Game::NXBUNSC},
        {"nxbunsc", Game::NXBUNSC},
    };

    str = to_lowercase(str);
    auto iterator = string_game_map.find(str);
    if (iterator != string_game_map.end())
        return iterator->second;
    return Game::UNKNOWN;
}