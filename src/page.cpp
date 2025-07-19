#include <nucc/page_new.hpp>
#include <nucc/chunk_new.hpp>

using namespace nucc;

void page::add(chunk chunk) {
    m_chunks.push_back(chunk);
}

const chunk& page::fetch(size_t index) const {
    return m_chunks[index];
}
const chunk& page::fetch(chunk_type type, size_t index) const {
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
const chunk& page::fetch(std::string_view name, size_t index) const {
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