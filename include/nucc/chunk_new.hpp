#ifndef KOJO_NUCC_CHUNK
#define KOJO_NUCC_CHUNK

#include <nucc/chunk_type.hpp>
#include <nucc/chunks/chunk_meta.hpp>

#include <nucc/chunks/chunk_null.hpp>
#include <nucc/chunks/chunk_page.hpp>
#include <nucc/chunks/chunk_binary.hpp>

#include <kojo/binary.hpp>
#include <kojo/logger.hpp>

#include <memory>

namespace nucc {

class xfbin;

class chunk {
public:
    chunk() = default;
    chunk(kojo::binary_view& input_data, const xfbin* xfbin);

    void load(kojo::binary_view& input_data, const xfbin* xfbin);

    chunk_type type() const { return m_type; }
    std::string type_string() const { return chunk_type_to_string(m_type); }
    std::string path() const { return m_path; }
    std::string name() const { return m_name; }
    std::uint32_t version() const { return m_version; }

    template<typename T>
    const T* meta() const { return dynamic_cast<T*>(m_meta.get()); }
    
    size_t size() const { return m_data.size(); }
    const std::byte* data() const { return m_data.data(); }
    const kojo::binary* storage() const { return &m_data; }

private:
    kojo::logger log{"NUCC++ Library"};

    chunk_type m_type{chunk_type::null};
    std::string m_path;
    std::string m_name;
    std::uint32_t m_version;

    std::shared_ptr<chunk_meta> m_meta;

    kojo::binary m_data;
};

}

#endif // KOJO_NUCC_CHUNK