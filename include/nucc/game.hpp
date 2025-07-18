#ifndef KOJO_NUCC_GAME
#define KOJO_NUCC_GAME

#include <string>

namespace nucc {

// List of CyberConnect2 games using the NUCC engine.
enum class game {
    // In order from old to new.
    unknown = 0,
    nsuns3, // Naruto Shippuden: Ultimate Ninja Storm 3 (2013)
    asb,    // JoJo's Bizarre Adventure: All-Star Battle (2013)
    nsunsr, // Naruto Shippuden: Ultimate Ninja Storm Revolution (2014)
    eohps3, // JoJo's Bizarre Adventure: Eyes of Heaven (2015) (PlayStation 3)
    eohps4, // JoJo's Bizarre Adventure: Eyes of Heaven (2015) (PlayStation 4)
    nsuns4, // Naruto Shippuden: Ultimate Ninja Storm 4 (2016)
    asbr,   // JoJo's Bizarre Adventure: All-Star Battle R (2022)
    nxbunsc // NARUTO X BORUTO Ultimate Ninja STORM CONNECTIONS (2023)
};

std::string game_to_string(game);
game string_to_game(std::string);

}

#endif // KOJO_NUCC_GAME