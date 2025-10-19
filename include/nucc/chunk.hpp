#ifndef KOJO_NUCC_CHUNK_HPP
#define KOJO_NUCC_CHUNK_HPP

#include <nucc/chunk_type.hpp>

#include <nucc/chunk_page.hpp>
#include <nucc/chunk_binary.hpp>
#include <nucc/chunk_texture.hpp>

#include <kojo/binary.hpp>
#include <kojo/logger.hpp>

#include <memory>

namespace nucc {

class xfbin;

class chunk {
public:
        chunk() = default;
        chunk(kojo::binary_view& input_data, xfbin* xfbin);
        explicit chunk(nucc::chunk_type _type, std::string_view _name =  "", std::string_view _path =  "")
                : m_type(_type), m_name(_name), m_path(_path) {}

        void load(kojo::binary_view& input_data, xfbin* xfbin);

        [[nodiscard]] chunk_type type() const { return m_type; }
        [[nodiscard]] std::string type_string() const { return chunk_type_to_string(m_type); }
        [[nodiscard]] std::string name() const { return m_name; }
        [[nodiscard]] std::string path() const { return m_path; }
        [[nodiscard]] std::uint32_t version() const { return m_version; }

        void set_type(chunk_type _type) { m_type = _type; }
        void set_name(std::string_view _name) { m_name = _name; }
        void set_path(std::string_view _path) { m_path = _path; }
        void set_version(std::uint32_t _version) { m_version = _version; }

        template<typename T>
        const T* meta(const std::source_location& loc = std::source_location::current()) const
        { 
                // !!! Need to complete error checking here.
                if constexpr (std::is_same_v<T, chunk_page>) {
                        if (m_type != chunk_type::page) {
                                log.fatal(
                                        kojo::logger::status::type_mismatch,
                                        std::format("Told to return chunk meta of `nucc::chunk_page`, but chunk's type is {}.", type_string()),
                                        "Specify a different type that matches the chunk's own type.",
                                        loc
                                );
                                return nullptr;
                        }
                } else if constexpr (std::is_same_v<T, chunk_texture>) {
                        if (m_type != chunk_type::texture) {
                                log.fatal(
                                        kojo::logger::status::type_mismatch,
                                        std::format("Told to return chunk meta of `nucc::chunk_texture`, but chunk's type is {}.", type_string()),
                                        "Specify a different type that matches the chunk's own type.",
                                        loc
                                );
                                return nullptr;
                        }
                } else if constexpr (std::is_same_v<T, chunk_binary>) {
                        if (m_type != chunk_type::binary) {
                                log.fatal(
                                        kojo::logger::status::type_mismatch,
                                        std::format("Told to return chunk meta of `nucc::chunk_binary`, but chunk's type is {}.", type_string()),
                                        "Specify a different type that matches the chunk's own type.",
                                        loc
                                );
                                return nullptr;
                        }
                } else {
                        log.fatal(
                                kojo::logger::status::bad_value,
                                std::format("Unsupported chunk type `{}`.", type_string()),
                                "Contact the author of this library to add support for this known chunk type.",
                                loc
                        );
                }

                return dynamic_cast<T*>(m_meta.get()); 
        }

        [[nodiscard]] size_t size() const { return m_data.size(); }
        [[nodiscard]] const std::byte* data() const { return m_data.data(); }
        [[nodiscard]] const kojo::binary* storage() const { return &m_data; }

        void update_data(const std::byte* _data, size_t size);

private:
        kojo::logger log{"NUCC++ Library", true, true};

        chunk_type m_type{chunk_type::null};
        std::string m_name;
        std::string m_path;
        std::uint32_t m_version{121};

        std::shared_ptr<chunk_meta> m_meta;

        kojo::binary m_data;
};

}

#endif