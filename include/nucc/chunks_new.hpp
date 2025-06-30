#ifndef KOJO_NUCC_CHUNKS
#define KOJO_NUCC_CHUNKS

#include <nucc/chunk_types.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class XFBIN;

class Chunk {
public:
    Chunk() = default;
    Chunk(const std::byte* input, size_t position, const XFBIN* xfbin);

    void load(const std::byte* input, size_t position, const XFBIN* xfbin);

    ChunkType type() const { return m_type; }
    std::string path() const { return m_path; }
    std::string name() const { return m_name; }
    std::uint32_t version() const { return m_version; }
    
    size_t size() const { return data.size(); }

private:
    ChunkType m_type{ChunkType::Null};
    std::string m_path;
    std::string m_name;
    std::uint32_t m_version;

    kojo::binary data;
};

}

#endif // KOJO_NUCC_CHUNKS