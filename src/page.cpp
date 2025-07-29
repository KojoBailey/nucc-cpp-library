#include <nucc/page.hpp>

using namespace nucc;

void page::add_chunk(chunk& chunk) {
    m_chunks.push_back(chunk);
}

const chunk& page::get_chunk(size_t index) const {
    return m_chunks[index];
}
const chunk& page::get_chunk(chunk_type type, size_t index) const {
    size_t i = 0;
    for (const chunk& chunk : m_chunks) {
        if (chunk.type() == type) {
            if (i == index)
                return chunk;
            i++;
        }
    }
    return fetch_last();
}
const chunk& page::get_chunk(std::string_view name, size_t index) const {
    size_t i = 0;
    for (const chunk& chunk : m_chunks) {
        if (chunk.path() == name || chunk.name() == name) {
            if (i == index)
                return chunk;
            i++;
        }
    }
    return fetch_last();
}

const chunk& page::fetch_last() const {
    return m_chunks[m_chunks.size() - 1];
}

bool page::has(std::string_view chunk_name) const {
    for (const chunk& m_chunk : m_chunks) {
        if (m_chunk.name() == chunk_name)
            return true;
    }
    return false;
}
bool page::has(chunk_type chunk_type) const {
    for (const chunk& m_chunk : m_chunks) {
        if (m_chunk.type() == chunk_type)
            return true;
    }
    return false;
}