#include "xfbin.hpp"

#include <unordered_map>
#include <unordered_set>

namespace nucc {

/** Constructors */

XFBIN::XFBIN() {}
XFBIN::XFBIN(std::filesystem::path input_path) {
    load(input_path);
}
XFBIN::XFBIN(kojo::binary& input_data, size_t start, size_t end) {
    load(input_data, start, end);
}
XFBIN::XFBIN(void* pointer_data, size_t start, size_t end) {
    load(pointer_data, start, end);
}

/** Loading */

void XFBIN::load(std::filesystem::path input_path) {
    input.load(input_path.string());
    if (input.data() == nullptr) {
        status = Error_Code::INVALID_FILE;
        error();
        return;
    }
    name = input_path.stem().string();
    read();
}
void XFBIN::load(kojo::binary& input_data, size_t start, size_t end) {
    if (input_data.data() == nullptr) {
        status = Error_Code::NULL_INPUT; 
        error();
        return;
    }
    input.load(input_data, start, end);
    read();
}
void XFBIN::load(void* pointer_data, size_t start, size_t end) {
    if (pointer_data == nullptr) {
        status = Error_Code::NULL_INPUT;
        error();
        return;
    }
    input.load(pointer_data, start, end);
    read();
}

/** Error Handling */

XFBIN::Status XFBIN::get_status() {
    std::string buffer;
    switch (status) {
        case Error_Code::OK             : buffer = "No issues detected."; break;
        case Error_Code::INVALID_FILE   : buffer = "File could not be loaded."; break;
        case Error_Code::NULL_INPUT     : buffer = "Input data is null."; break;
        case Error_Code::MAGIC          : buffer = "File magic does not match `NUCC` or `4E 55 43 43`."; break;
        case Error_Code::VERSION        : buffer = "XFBIN version not supported. Must be `121`."; break;
        default                         : buffer = "Invalid error code.";
    }
    return {
        status, (int)status, buffer
    };
}

void XFBIN::error() {
    Status buffer = get_status();
    std::string number = std::to_string(buffer.number);
    while (number.length() < 3)
        number = "0" + number;
    std::cout << "[NUCC error " << number << "] " << buffer.message << "\n";
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
    str_to_ChunkType["nuccChunkDynamic"]    = Chunk_Type::Dynamic;
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

void XFBIN::read() {
    // Deserialise header.
    if (magic != input.read<std::string>(4)) {
        status = Error_Code::MAGIC;
        error();
        return;
    }

    version = input.read<std::uint32_t>(kojo::endian::big);
    if (version != 121) {
        status = Error_Code::VERSION;
        error();
        return;
    }

    input.read<std::uint64_t>(kojo::endian::big); /** Flags. @note Parse these at some point. */

    // Deserialise Index.
        input.change_pos(12); // Chunk header (useless for Index).
        index.type_count          = input.read<std::uint32_t>(kojo::endian::big);
        index.type_size           = input.read<std::uint32_t>(kojo::endian::big);
        index.path_count          = input.read<std::uint32_t>(kojo::endian::big);
        index.path_size           = input.read<std::uint32_t>(kojo::endian::big);
        index.name_count          = input.read<std::uint32_t>(kojo::endian::big);
        index.name_size           = input.read<std::uint32_t>(kojo::endian::big);
        index.map_count           = input.read<std::uint32_t>(kojo::endian::big);
        index.map_size            = input.read<std::uint32_t>(kojo::endian::big);
        index.map_indices_count   = input.read<std::uint32_t>(kojo::endian::big);
        index.extra_indices_count = input.read<std::uint32_t>(kojo::endian::big);

        // Store strings.
            for (int i = 0; i < index.type_count; i++)
                index.types.push_back(input.read<std::string>());
            for (int i = 0; i < index.path_count; i++)
                index.paths.push_back(input.read<std::string>());
            for (int i = 0; i < index.name_count; i++)
                index.names.push_back(input.read<std::string>());

        input.align_by(4);

        // Store maps.
        for (int i = 0; i < index.map_count; i++)
            index.maps.push_back({
                input.read<std::uint32_t>(kojo::endian::big),
                input.read<std::uint32_t>(kojo::endian::big),
                input.read<std::uint32_t>(kojo::endian::big)
            });
        for (int i = 0; i < index.extra_indices_count; i++)
            index.extra_indices.push_back({
                input.read<std::uint32_t>(kojo::endian::big),
                input.read<std::uint32_t>(kojo::endian::big)
            });
        for (int i = 0; i < index.map_indices_count; i++)
            index.map_indices.push_back(input.read<std::uint32_t>(kojo::endian::big));

    // Read chunks.
    pages.clear();
    for (int page_it = 0; !input.at_end(); page_it++) {
        pages.emplace_back();
        auto& page = pages[page_it];

        for (int i = 0; true; i++) {
            Chunk chunk;
            chunk.load(input.data(), input.get_pos());
            chunk.type = index.get_type(chunk.map_index);
            chunk.path = index.get_path(chunk.map_index);
            chunk.name = index.get_name(chunk.map_index);
            input.change_pos(chunk.size + 12); // 12 being size of header
            if (chunk.type == Chunk_Type::Page) {
                page.load(&chunk);
                index.running_map_offset += page.content.map_offset;
                index.running_extra_offset += page.content.extra_offset;
                break;
            }
            page.chunks.push_back(&chunk);
        }
    }
}

Chunk* XFBIN::fetch(std::string chunk_name, size_t index) {
    size_t i = 0;
    for (auto& page : pages) {
        for (auto& chunk : page.chunks) {
            if (chunk.name == chunk_name || chunk.path == chunk_name) {
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
            if (!(optimize == Optimize::TRUE && chunk.type == Chunk_Type::Null)) {
                std::string type = chunk.type_as_string();
                if ( type_tracker.count(type) == 0 ) {
                    type_tracker[type] = type_tracker.size();
                    index.types.push_back(type);
                    index.type_size += type.size() + 1;
                }
                if ( path_tracker.count(chunk.path) == 0 ) {
                    path_tracker[chunk.path] = path_tracker.size();
                    index.paths.push_back(chunk.path);
                    index.path_size += chunk.path.size() + 1;
                }
                if ( name_tracker.count(chunk.name) == 0 ) {
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
}

void XFBIN::write(std::string output_path, Optimize optimize) {
    output.write<std::string>(magic, 4);
    output.write<std::uint32_t>(version, kojo::endian::big);
    output.write<std::uint64_t>(0, kojo::endian::big);

        output.write<std::uint32_t>(0, kojo::endian::big);
        output.write<std::uint32_t>(0, kojo::endian::big);
        output.write<std::uint16_t>(0, kojo::endian::big);
        output.write<std::uint16_t>(0, kojo::endian::big);
        calculate(optimize);
        output.write<std::uint32_t>(index.type_count,             kojo::endian::big);
        output.write<std::uint32_t>(index.type_size,              kojo::endian::big);
        output.write<std::uint32_t>(index.path_count,             kojo::endian::big);
        output.write<std::uint32_t>(index.path_size,              kojo::endian::big);
        output.write<std::uint32_t>(index.name_count,             kojo::endian::big);
        output.write<std::uint32_t>(index.name_size,              kojo::endian::big);
        output.write<std::uint32_t>(index.map_count,              kojo::endian::big);
        output.write<std::uint32_t>(index.map_size,               kojo::endian::big);
        output.write<std::uint32_t>(index.map_indices_count,      kojo::endian::big);
        output.write<std::uint32_t>(index.extra_indices_count,    kojo::endian::big);

        // Write strings.
        for (auto& type : index.types) output.write<std::string>(type);
        for (auto& path : index.paths) output.write<std::string>(path);
        for (auto& name : index.names) output.write<std::string>(name);

        output.align_by(4);

        // Write maps.
        for (auto& map : index.maps) {
            output.write<std::uint32_t>(map.type_index, kojo::endian::big);
            output.write<std::uint32_t>(map.path_index, kojo::endian::big);
            output.write<std::uint32_t>(map.name_index, kojo::endian::big);
        }
        for (auto& extra_indices_group : index.extra_indices) {
            output.write<std::uint32_t>(extra_indices_group.name_index, kojo::endian::big);
            output.write<std::uint32_t>(extra_indices_group.map_index, kojo::endian::big);
        }
        for (auto& map_index : index.map_indices) {
            output.write<std::uint32_t>(map_index, kojo::endian::big);
        }

    /* nuccChunk */
    for (auto& page : pages) {
        for (auto& chunk : page.chunks) {
            if (!(optimize == Optimize::TRUE && chunk.type == Chunk_Type::Null)) {
                output.write<std::uint32_t>(chunk.size, kojo::endian::big);
                output.write<std::uint32_t>(chunk.map_index, kojo::endian::big);
                output.write<std::uint16_t>(chunk.version, kojo::endian::big);
                output.write<std::uint16_t>(chunk.unk, kojo::endian::big);
                output.write<kojo::binary>(chunk.dump());
            }
        }
    }

    output.dump_file(output_path);
}

} // namespace nucc