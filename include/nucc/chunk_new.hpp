#ifndef KOJO_NUCC_CHUNK
#define KOJO_NUCC_CHUNK

#include <nucc/chunk_type.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class XFBIN;

class Chunk {
public:
    Chunk() = default;
    Chunk(const std::byte* input, size_t position, const XFBIN* xfbin);

    void load(const std::byte* input, size_t position, const XFBIN* xfbin);

    ChunkType type() const { return m_type; }
    std::string type_string() const { return chunk_type_to_string(m_type); }
    std::string path() const { return m_path; }
    std::string name() const { return m_name; }
    std::uint32_t version() const { return m_version; }
    
    size_t size() const { return m_data.size(); }
    const std::byte* data() const { return m_data.data(); }

private:
    ChunkType m_type{ChunkType::Null};
    std::string m_path;
    std::string m_name;
    std::uint32_t m_version;

    kojo::binary m_data;
};

}

#endif // KOJO_NUCC_CHUNK