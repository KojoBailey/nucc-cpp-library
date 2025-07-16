#ifndef KOJO_NUCC_BINARY
#define KOJO_NUCC_BINARY

#include "chunks.hpp"
#include "binary/binary_data.hpp"

namespace nucc {

// Contains miscellaneous binary data, including formats not explicitly supported like XML.
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
        content.binary_data.load(input.write_to_bin(), 0, content.size);
        update();
    }

    unsigned char* data() {
        return content.binary_data.data();
    }

    void update() {
        storage.clear();
        storage.write_int<std::uint32_t>(content.size, std::endian::big);
        storage.write_binary(content.binary_data);
        type = Chunk_Type::Binary;
        size = content.size + 4;
    }

    kojo::binary& dump() {
        return storage;
    }

private:
    void parse() {
        storage.set_pos(0);
        content.size = storage.read_int<std::uint32_t>(std::endian::big);
        content.binary_data.load(storage, 4);
    }
};

}

#endif // KOJO_NUCC_BINARY