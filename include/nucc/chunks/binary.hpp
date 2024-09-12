#ifndef KOJO_NUCC_BINARY
#define KOJO_NUCC_BINARY

#include "chunks.hpp"
#include "binary/binary_data.hpp"

namespace nucc {

/**
 * Contains miscellaneous binary data, including formats not explicitly supported like XML.
*/
class Binary : public Chunk {
public:
    struct {
        std::uint32_t size{0};
        kojo::binary binary_data;
    } content;

    Binary() = default;
    Binary(Chunk* chunk) {
        load(chunk);
    }
    Binary(std::string path_input, std::string name_input) {
        path = path_input;
        name = name_input;
    }

    void load_data(Binary_Data& input) {
        content.size = input.size();
        content.binary_data.load(input.write());
        update();
    }

    unsigned char* data() {
        return content.binary_data.data();
    }

    void update() {
        storage.clear();
        storage.write<std::uint32_t>(content.size, kojo::endian::big);
        for (int i = 0; i < content.binary_data.size(); i++) {
            storage.write<char>(content.binary_data.read<char>());
        }
        type = Chunk_Type::Binary;
    }

    kojo::binary& dump() {
        return storage;
    }

private:
    void parse() {
        storage.set_pos(0);
        content.size = storage.read<std::uint32_t>(kojo::endian::big);
        content.binary_data.load(storage, 4);
    }
};

}

#endif // KOJO_NUCC_BINARY