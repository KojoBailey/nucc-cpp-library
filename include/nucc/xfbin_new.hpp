#ifndef KOJO_NUCC_XFBIN
#define KOJO_NUCC_XFBIN

#include <nucc/game.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class XFBIN {
public:
    std::string filename;
    Game game{Game::UNKNOWN};

    static constexpr std::string_view MAGIC{"NUCC"}; // Required of an XFBIN to start with these 4 bytes.
    std::uint32_t version{121}; // e.g. `121` = 1.2.1

    XFBIN() = default;
    XFBIN(const std::filesystem::path input_path);
    XFBIN(const std::byte* src);
    XFBIN(const kojo::binary& binary);
    ~XFBIN() = default;

    void load(const std::filesystem::path input_path);
    void load(const std::byte* input_memory);
    void load(const kojo::binary& input_binary);

private:

};

}

#endif // KOJO_NUCC_XFBIN