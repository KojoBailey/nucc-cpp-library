#ifndef KOJO_NUCC_CHUNK
#define KOJO_NUCC_CHUNK

#include <nucc/chunk_type.hpp>
#include <nucc/chunks/chunk_meta.hpp>

#include <kojo/binary.hpp>

namespace nucc {

class xfbin;

class chunk {
public:
    chunk() = default;
    chunk(const std::byte* input, size_t position, const xfbin* xfbin);

    void load(const std::byte* input, size_t position, const xfbin* xfbin);

    chunk_type type() const { return m_type; }
    std::string type_string() const { return chunk_type_to_string(m_type); }
    std::string path() const { return m_path; }
    std::string name() const { return m_name; }
    std::uint32_t version() const { return m_version; }
    
    size_t size() const { return m_data.size(); }
    const std::byte* data() const { return m_data.data(); }

private:
    chunk_type m_type{chunk_type::null};
    std::string m_path;
    std::string m_name;
    std::uint32_t m_version;

    chunk_meta* m_meta;

    kojo::binary m_data;
};

}

#endif // KOJO_NUCC_CHUNK