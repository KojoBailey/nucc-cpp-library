#ifndef KOJO_XFBIN_INDEX
#define KOJO_XFBIN_INDEX

#include <kojo/binary.hpp>

namespace kojo::nucc {

struct NuccIndexSizes {
	std::uint32_t chunk_type_count;
	std::uint32_t chunk_type_size;
	std::uint32_t file_path_count;
	std::uint32_t file_path_size;
	std::uint32_t chunk_name_count;
	std::uint32_t chunk_name_size;
	std::uint32_t chunk_map_count;
	std::uint32_t chunk_map_size;
	std::uint32_t chunk_map_indices_count;
	std::uint32_t extra_map_indices_count;
};

}

#endif
