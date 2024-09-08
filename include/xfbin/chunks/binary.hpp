#include "chunks.hpp"

namespace nucc {

class Binary : public Chunk {
public:
    struct {
        std::uint32_t size{0};
        kojo::binary binary_data;
    } Data;

    Binary() {
        type = ChunkType::Binary;
    }

    void load(kojo::binary& input) {
        input.set_pos(0);
        Data.size = input.read<std::uint32_t>(kojo::endian::big);
        Data.binary_data.load(input, 4);
    }
    kojo::binary dump() {
        kojo::binary output;
        output.write<std::uint32_t>(Data.size, kojo::endian::big);
        for (int i = 0; i < Data.binary_data.size(); i++) {
            output.write<char>(Data.binary_data.read<char>());
        }
        output.set_pos(0);
        return &output;
    }
};

}