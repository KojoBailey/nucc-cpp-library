#include <nucc/xfbin_new.hpp>
#include <nucc/error_handling.hpp>

#include <unordered_map>
#include <unordered_set>

using namespace nucc;
using namespace kojo::binary_types;

XFBIN::XFBIN(const std::filesystem::path input_path) {
    load(input_path);
}
XFBIN::XFBIN(kojo::binary_view input_binary, size_t _size) {
    load(input_binary, _size);
}

void XFBIN::load(const std::filesystem::path input_path) {
    kojo::binary input_data{input_path};
    size = input_data.size();
    if (input_data.is_empty())
        Error::print({
            Status_Code::FILE_NULL,
            std::format("Could not load file at path `{}` into `nucc::XFBIN` object.", input_path.string()),
            "Ensure the file specified exists and is a valid XFBIN."
        });
    m_filename = input_path.stem().string();
    read(input_data);
}
void XFBIN::load(kojo::binary_view input_binary, size_t _size) {
    if (input_binary.is_empty())
        Error::print({
            Status_Code::POINTER_NULL,
            "Could not load from a null pointer into `nucc::XFBIN` object.",
            "Ensure the address provided contains appropiate data."
        });
    size = _size;
    read(input_binary);
}

void XFBIN::read(kojo::binary_view data) {
    read_header(data);
    read_index(data);
    read_chunks(data);
}
void XFBIN::read_header(kojo::binary_view data) {
    auto magic_input = data.read<sv>(4);
    if (magic_input != MAGIC) {
        Error::print({
            Status_Code::FILE_MAGIC,
            std::format("When reading XFBIN `{}`, expected magic `{}` but instead got `{}`.", m_filename, MAGIC, magic_input),
            std::format("Ensure the file's signature is `{}`, and that it is indeed an XFBIN file.", MAGIC)
        });
        return;
    }

    auto version_input = data.read<u32>(std::endian::big);
    if (version_input != VERSION) {
        Error::print({
            Status_Code::VERSION,
            std::format("When reading XFBIN `{}`, expected version `{}` but instead got `{}`.", m_filename, VERSION, version_input),
            std::format("Ensure the XFBIN's version is `{}`.", VERSION)
        });
        return;
    }

    data.read<u64>(std::endian::big); // Flags. Parse these at some point.
}
void XFBIN::read_index(kojo::binary_view data) {
    data.change_pos(HEADER_SIZE); // Chunk header (useless for Index).
    auto type_count = data.read<u32>(std::endian::big);
    auto type_size = data.read<u32>(std::endian::big);
    auto path_count = data.read<u32>(std::endian::big);
    auto path_size = data.read<u32>(std::endian::big);
    auto name_count = data.read<u32>(std::endian::big);
    auto name_size = data.read<u32>(std::endian::big);
    auto map_count = data.read<u32>(std::endian::big);
    auto map_size = data.read<u32>(std::endian::big);
    auto map_indices_count = data.read<u32>(std::endian::big);
    auto extra_indices_count = data.read<u32>(std::endian::big);

    // Store strings.
    for (size_t i = 0; i < type_count; i++)
        types.push_back(data.read<sv>());
    for (size_t i = 0; i < path_count; i++)
        paths.push_back(data.read<sv>());
    for (size_t i = 0; i < name_count; i++)
        names.push_back(data.read<sv>());

    data.align_by(4);

    // Store maps.
    for (int i = 0; i < map_count; i++)
        maps.push_back({
            data.read<u32>(std::endian::big),
            data.read<u32>(std::endian::big),
            data.read<u32>(std::endian::big)
        });
    for (int i = 0; i < extra_indices_count; i++)
        extra_indices.push_back({
            data.read<u32>(std::endian::big),
            data.read<u32>(std::endian::big)
        });
    for (int i = 0; i < map_indices_count; i++)
        map_indices.push_back(data.read<u32>(std::endian::big));
}
void XFBIN::read_chunks(kojo::binary_view data) {
    pages.clear();
    for (size_t page_it = 0; !(data.get_pos() >= size); page_it++) {
        pages.emplace_back();
        auto& page = pages[page_it];

        while (!(data.get_pos() >= size)) {
            Chunk chunk{data.data(), data.get_pos(), this};
            data.change_pos(chunk.size() + HEADER_SIZE);
            if (chunk.type() == ChunkType::Page) {
                auto map_offset = data.read<u32>(std::endian::big);
                auto extra_offset = data.read<u32>(std::endian::big);
                running_map_offset += map_offset;
                running_extra_offset += extra_offset;
                break;
            }
            page.add(chunk);
        }
    }
}

/** Other Functions */

Chunk_Type XFBIN::Index::get_type(std::uint32_t map_index) {
    std::unordered_map<std::string, Chunk_Type> str_to_ChunkType;
    str_to_ChunkType["nuccChunkNull"]       = Chunk_Type::Null;
    str_to_ChunkType["nuccChunkIndex"]      = Chunk_Type::Index;
    str_to_ChunkType["nuccChunkPage"]       = Chunk_Type::Page;
    str_to_ChunkType["nuccChunkAnm"]        = Chunk_Type::Anm;
    str_to_ChunkType["nuccChunkBillboard"]  = Chunk_Type::Billboard;
    str_to_ChunkType["nuccChunkBinary"]     = Chunk_Type::Binary;
    str_to_ChunkType["nuccChunkCamera"]     = Chunk_Type::Camera;
    str_to_ChunkType["nuccChunkClump"]      = Chunk_Type::Clump;
    str_to_ChunkType["nuccChunkCoord"]      = Chunk_Type::Coord;
    str_to_ChunkType["nuccChunkDynamics"]   = Chunk_Type::Dynamics;
    str_to_ChunkType["nuccChunkLightDirc"]  = Chunk_Type::LightDirc;
    str_to_ChunkType["nuccChunkMaterial"]   = Chunk_Type::Material;
    str_to_ChunkType["nuccChunkModel"]      = Chunk_Type::Model;
    str_to_ChunkType["nuccChunkNub"]        = Chunk_Type::Nub;
    str_to_ChunkType["nuccChunkParticle"]   = Chunk_Type::Particle;
    str_to_ChunkType["nuccChunkSprite"]     = Chunk_Type::Sprite;
    str_to_ChunkType["nuccChunkTexture"]    = Chunk_Type::Texture;
    str_to_ChunkType["nuccChunkTrail"]      = Chunk_Type::Trail;

    return str_to_ChunkType[types[maps[map_indices[map_index + running_map_offset]].type_index]];
}
std::string XFBIN::Index::get_path(std::uint32_t map_index) {
    return paths[maps[map_indices[map_index + running_map_offset]].path_index];
}
std::string XFBIN::Index::get_name(std::uint32_t map_index) {
    return names[maps[map_indices[map_index + running_map_offset]].name_index];
}

Chunk* XFBIN::fetch(Chunk_Type chunk_type, size_t index) {
    size_t i = 0;
    for (auto& page : pages) {
        for (auto& chunk : page.chunks) {
            if (chunk.type == chunk_type) {
                if (i == index) {
                    return &chunk;
                } else {
                    i++;
                }
            }
        }
    }
    return nullptr;
}

void XFBIN::calculate(Optimize optimize) {
    std::unordered_map<std::string, std::uint32_t> type_tracker;
    std::unordered_map<std::string, std::uint32_t> path_tracker;
    std::unordered_map<std::string, std::uint32_t> name_tracker;
    std::unordered_map<std::string, std::uint32_t> map_tracker;
    index.types.clear();
    index.paths.clear();
    index.names.clear();
    index.maps.clear();
    index.type_size = 0;
    index.path_size = 0;
    index.name_size = 0;
    index.map_size = 0;

    for (auto& page : pages) {
        for (auto& chunk : page.chunks) {
            if (!(optimize == Optimize::YES && chunk.type == Chunk_Type::Null)) {
                std::string type = chunk.type_as_string();
                if (type_tracker.count(type) == 0) {
                    type_tracker[type] = type_tracker.size();
                    index.types.push_back(type);
                    index.type_size += type.size() + 1;
                }
                if (path_tracker.count(chunk.path) == 0) {
                    path_tracker[chunk.path] = path_tracker.size();
                    index.paths.push_back(chunk.path);
                    index.path_size += chunk.path.size() + 1;
                }
                if (name_tracker.count(chunk.name) == 0) {
                    name_tracker[chunk.name] = name_tracker.size();
                    index.names.push_back(chunk.name);
                    index.name_size += chunk.name.size() + 1;
                }

                Index::Chunk_Map temp_map = {type_tracker[type], path_tracker[chunk.path], name_tracker[chunk.name]};
                std::string map_string = std::to_string(temp_map.type_index)
                    + "-" + std::to_string(temp_map.path_index)
                    + "-" + std::to_string(temp_map.name_index);
                if ( map_tracker.count(map_string) == 0 ) {
                    map_tracker[map_string] = map_tracker.size();
                    index.maps.push_back(temp_map);
                    index.map_size += sizeof(Index::Chunk_Map);
                    index.map_indices.push_back(map_tracker[map_string]);
                }
                chunk.map_index = index.map_indices[map_tracker[map_string]];
            }
        }

    }

    index.type_count = index.types.size();
    index.path_count = index.paths.size();
    index.name_count = index.names.size();
    index.map_count = index.maps.size();
    index.map_indices_count = index.map_indices.size();
    index.extra_indices_count = 0;
}

void XFBIN::write(std::string output_path, Optimize optimize) {
    output.write_str(magic, 4);
    output.write_int<std::uint32_t>(version, std::endian::big);
    output.write_int<std::uint64_t>(0, std::endian::big);

        output.write_int<std::uint32_t>(0, std::endian::big);
        output.write_int<std::uint32_t>(0, std::endian::big);
        output.write_int<std::uint16_t>(0, std::endian::big);
        output.write_int<std::uint16_t>(0, std::endian::big);
        calculate(optimize);
        output.write_int<std::uint32_t>(index.type_count,             std::endian::big);
        output.write_int<std::uint32_t>(index.type_size,              std::endian::big);
        output.write_int<std::uint32_t>(index.path_count,             std::endian::big);
        output.write_int<std::uint32_t>(index.path_size,              std::endian::big);
        output.write_int<std::uint32_t>(index.name_count,             std::endian::big);
        output.write_int<std::uint32_t>(index.name_size,              std::endian::big);
        output.write_int<std::uint32_t>(index.map_count,              std::endian::big);
        output.write_int<std::uint32_t>(index.map_size,               std::endian::big);
        output.write_int<std::uint32_t>(index.map_indices_count,      std::endian::big);
        output.write_int<std::uint32_t>(index.extra_indices_count,    std::endian::big);

        // Write strings.
        for (auto& type : index.types) output.write_str(type);
        for (auto& path : index.paths) {
            if (path == "") output.write_char('\0');
            else output.write_str(path);
        }
        for (auto& name : index.names) {
            if (name == "") output.write_char('\0');
            else output.write_str(name);
        }

        output.align_by(4);

        // Write maps.
        for (auto& map : index.maps) {
            output.write_int<std::uint32_t>(map.type_index, std::endian::big);
            output.write_int<std::uint32_t>(map.path_index, std::endian::big);
            output.write_int<std::uint32_t>(map.name_index, std::endian::big);
        }
        for (auto& extra_indices_group : index.extra_indices) {
            output.write_int<std::uint32_t>(extra_indices_group.name_index, std::endian::big);
            output.write_int<std::uint32_t>(extra_indices_group.map_index, std::endian::big);
        }
        for (auto& map_index : index.map_indices) {
            output.write_int<std::uint32_t>(map_index, std::endian::big);
        }

    /* nuccChunk */
    for (auto& page : pages) {
        for (auto& chunk : page.chunks) {
            if (!(optimize == Optimize::YES && chunk.type == Chunk_Type::Null)) {
                output.write_int<std::uint32_t>(chunk.size, std::endian::big);
                output.write_int<std::uint32_t>(chunk.map_index, std::endian::big);
                output.write_int<std::uint16_t>(chunk.version, std::endian::big);
                output.write_int<std::uint16_t>(chunk.unk, std::endian::big);
                output.write_binary(chunk.dump());
            }
        }
    }

    output.dump_file(output_path);
}