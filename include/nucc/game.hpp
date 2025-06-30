#ifndef KOJO_NUCC_GAME
#define KOJO_NUCC_GAME

#include <string>

namespace nucc {

// List of CyberConnect2 games using the NUCC engine.
enum class Game {
    // In order from old to new.
    UNKNOWN = 0,
    NSUNS3, // Naruto Shippuden: Ultimate Ninja Storm 3 (2013)
    ASB,    // JoJo's Bizarre Adventure: All-Star Battle (2013)
    NSUNSR, // Naruto Shippuden: Ultimate Ninja Storm Revolution (2014)
    EOHPS3, // JoJo's Bizarre Adventure: Eyes of Heaven (2015) (PlayStation 3)
    EOHPS4, // JoJo's Bizarre Adventure: Eyes of Heaven (2015) (PlayStation 4)
    NSUNS4, // Naruto Shippuden: Ultimate Ninja Storm 4 (2016)
    ASBR,   // JoJo's Bizarre Adventure: All-Star Battle R (2022)
    NXBUNSC // NARUTO X BORUTO Ultimate Ninja STORM CONNECTIONS (2023)
};

std::string game_to_string(Game);
Game string_to_game(std::string);

}

#endif // KOJO_NUCC_GAME