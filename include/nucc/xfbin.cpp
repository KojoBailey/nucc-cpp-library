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

int XFBIN::load(std::filesystem::path input_path) {
    input.load(input_path.string());
    if (input.data() == nullptr)
        return error_handler({
            Status_Code::FILE_NULL,
            std::format("Could not load file at path `{}` into `nucc::XFBIN` object.", input_path.string()),
            "Ensure the file specified exists and is a valid XFBIN."
        });
    name = input_path.stem().string();
    read();
    return 0;
}
int XFBIN::load(kojo::binary& input_data, size_t start, size_t end) {
    if (input_data.data() == nullptr)
        return error_handler({
            Status_Code::POINTER_NULL,
            "Could not load null data from `kojo::binary` object into `nucc::XFBIN` object.",
            "Ensure the object provided contains accessible data."
        });
    input.load(input_data, start, end);
    read();
    return 0;
}
int XFBIN::load(void* pointer_data, size_t start, size_t end) {
    if (pointer_data == nullptr)
        return error_handler({
            Status_Code::POINTER_NULL,
            "Could not load from a null pointer into `nucc::XFBIN` object.",
            "Ensure the address provided contains appropiate data."
        });
    input.load(pointer_data, start, end);
    read();
    return 0;
}

/** Other Functions */

std::string game_to_string(Game game) {
    switch (game) {
        case Game::NSUNS3   : return "Naruto Shippuden: Ultimate Ninja Storm 3";
        case Game::ASB      : return "JoJo's Bizarre Adventure: All-Star Battle";
        case Game::NSUNSR   : return "Naruto Shippuden: Ultimate Ninja Storm Revolution";
        case Game::EOHPS3   : return "JoJo's Bizarre Adventure: Eyes of Heaven (PS3)";
        case Game::EOHPS4   : return "JoJo's Bizarre Adventure: Eyes of Heaven (PS4)";
        case Game::NSUNS4   : return "Naruto Shippuden: Ultimate Ninja Storm 4";
        case Game::ASBR     : return "JoJo's Bizarre Adventure: All-Star Battle R";
        case Game::NXBUNSC  : return "NARUTO X BORUTO Ultimate Ninja STORM CONNECTIONS";
    }
    return "Unknown";
}
Game string_to_game(std::string str) {
    // Convert to lowercase.
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c){ return std::tolower(c); });

    if (str == "naruto shippuden: ultimate ninja storm 3" || str == "nsuns3")
        return Game::NSUNS3;
    else if (str == "jojo's bizarre adventure: all-star battle" || str == "asb")
        return Game::ASB;
    else if (str == "naruto shippuden: ultimate ninja storm revolution" || str == "nsunsr")
        return Game::NSUNSR;
    else if (str == "jojo's bizarre adventure: eyes of heaven (ps3)" || str == "eohps3")
        return Game::EOHPS3;
    else if (str == "jojo's bizarre adventure: eyes of heaven (ps4)" || str == "eohps4")
        return Game::EOHPS4;
    else if (str == "naruto shippuden: ultimate ninja storm 4" || str == "nsuns4")
        return Game::NSUNS4;
    else if (str == "jojo's bizarre adventure: all-star battle r" || str == "asbr")
        return Game::ASBR;
    else if (str == "naruto x boruto ultimate ninja storm connections" || str == "nxbunsc")
        return Game::NXBUNSC;
    else
        return Game::UNKNOWN;
}

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

int XFBIN::read() {
    // Deserialise header.
    std::string magic_input = input.read<std::string>(4);
    if (magic != magic_input)
        return error_handler({
            Status_Code::FILE_MAGIC,
            std::format("When reading XFBIN `{}`, expected magic `{}` but instead got `{}`.", name, magic, magic_input),
            std::format("Ensure the file's signature is `{}`, and that it is indeed an XFBIN file.", magic)
        });

    version = input.read<std::uint32_t>(std::endian::big);
    if (version != 121)
        return error_handler({
            Status_Code::VERSION,
            std::format("When reading XFBIN `{}`, expected version `{}` but instead got `{}`.", name, 121, version),
            "Ensure the XFBIN's version is `121`."
        });

    input.read<std::uint64_t>(std::endian::big); /** Flags. @note Parse these at some point. */

    // Deserialise Index.
        input.change_pos(12); // Chunk header (useless for Index).
        index.type_count          = input.read<std::uint32_t>(std::endian::big);
        index.type_size           = input.read<std::uint32_t>(std::endian::big);
        index.path_count          = input.read<std::uint32_t>(std::endian::big);
        index.path_size           = input.read<std::uint32_t>(std::endian::big);
        index.name_count          = input.read<std::uint32_t>(std::endian::big);
        index.name_size           = input.read<std::uint32_t>(std::endian::big);
        index.map_count           = input.read<std::uint32_t>(std::endian::big);
        index.map_size            = input.read<std::uint32_t>(std::endian::big);
        index.map_indices_count   = input.read<std::uint32_t>(std::endian::big);
        index.extra_indices_count = input.read<std::uint32_t>(std::endian::big);

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
                input.read<std::uint32_t>(std::endian::big),
                input.read<std::uint32_t>(std::endian::big),
                input.read<std::uint32_t>(std::endian::big)
            });
        for (int i = 0; i < index.extra_indices_count; i++)
            index.extra_indices.push_back({
                input.read<std::uint32_t>(std::endian::big),
                input.read<std::uint32_t>(std::endian::big)
            });
        for (int i = 0; i < index.map_indices_count; i++)
            index.map_indices.push_back(input.read<std::uint32_t>(std::endian::big));

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
    
    return 0;
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
    output.write<std::uint32_t>(version, std::endian::big);
    output.write<std::uint64_t>(0, std::endian::big);

        output.write<std::uint32_t>(0, std::endian::big);
        output.write<std::uint32_t>(0, std::endian::big);
        output.write<std::uint16_t>(0, std::endian::big);
        output.write<std::uint16_t>(0, std::endian::big);
        calculate(optimize);
        output.write<std::uint32_t>(index.type_count,             std::endian::big);
        output.write<std::uint32_t>(index.type_size,              std::endian::big);
        output.write<std::uint32_t>(index.path_count,             std::endian::big);
        output.write<std::uint32_t>(index.path_size,              std::endian::big);
        output.write<std::uint32_t>(index.name_count,             std::endian::big);
        output.write<std::uint32_t>(index.name_size,              std::endian::big);
        output.write<std::uint32_t>(index.map_count,              std::endian::big);
        output.write<std::uint32_t>(index.map_size,               std::endian::big);
        output.write<std::uint32_t>(index.map_indices_count,      std::endian::big);
        output.write<std::uint32_t>(index.extra_indices_count,    std::endian::big);

        // Write strings.
        for (auto& type : index.types) output.write<std::string>(type);
        for (auto& path : index.paths) output.write<std::string>(path);
        for (auto& name : index.names) output.write<std::string>(name);

        output.align_by(4);

        // Write maps.
        for (auto& map : index.maps) {
            output.write<std::uint32_t>(map.type_index, std::endian::big);
            output.write<std::uint32_t>(map.path_index, std::endian::big);
            output.write<std::uint32_t>(map.name_index, std::endian::big);
        }
        for (auto& extra_indices_group : index.extra_indices) {
            output.write<std::uint32_t>(extra_indices_group.name_index, std::endian::big);
            output.write<std::uint32_t>(extra_indices_group.map_index, std::endian::big);
        }
        for (auto& map_index : index.map_indices) {
            output.write<std::uint32_t>(map_index, std::endian::big);
        }

    /* nuccChunk */
    for (auto& page : pages) {
        for (auto& chunk : page.chunks) {
            if (!(optimize == Optimize::YES && chunk.type == Chunk_Type::Null)) {
                output.write<std::uint32_t>(chunk.size, std::endian::big);
                output.write<std::uint32_t>(chunk.map_index, std::endian::big);
                output.write<std::uint16_t>(chunk.version, std::endian::big);
                output.write<std::uint16_t>(chunk.unk, std::endian::big);
                output.write<kojo::binary>(chunk.dump());
            }
        }
    }

    output.dump_file(output_path);
}

} // namespace nucc