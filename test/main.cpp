#include <xfbin/xfbin.hpp>
#include <xfbin/chunk_binary.hpp>
#include <xfbin/chunk_model.hpp>

#include <print>
#include <iostream>

using namespace kojo;

int main(int argc, char* argv[])
{
	std::println("Welcome to the XFBIN lib testing ground!\n");

	if (argc < 2) {
		std::println(std::cerr, "Too few arguments.");
		return 1;
	}

	const auto maybe_xfbin = nucc::Xfbin::from(argv[1]);
	if (!maybe_xfbin) {
		std::println("Error {:03}: {}",
				maybe_xfbin.error().to_code(),
				maybe_xfbin.error().to_string()
		);
		return 1;
	}
	const auto xfbin = std::move(*maybe_xfbin);

	std::println("Version: {}", xfbin.get_version());
	std::println();

	const nucc::Chunk* chunk1 = xfbin.fetch_chunk({}, {}, "PlayerColorParam");
	if (chunk1 == nullptr) {
		std::println(std::cerr, "[ERROR] Could not find specified chunk.");
	} else {
		const auto maybe_binary = nucc::ChunkBinary::from(*chunk1);
		if (!maybe_binary.has_value()) {
			std::println(std::cerr, "[ERROR] {}", maybe_binary.error().to_string());
			return 1;
		}
		const auto binary = std::move(*maybe_binary);
		std::println("Size: {}", binary.size);
	}
	std::println();

	const nucc::Chunk* chunk2 = xfbin.fetch_chunk(nucc::ChunkType::Model, {}, {});
	if (chunk2 == nullptr) {
		std::println(std::cerr, "[ERROR] Could not find specified chunk.");
	} else {
		const auto maybe_model = nucc::ChunkModel::from(*chunk2);
		if (!maybe_model.has_value()) {
			std::println(std::cerr, "[ERROR] {}", maybe_model.error().to_string());
			return 1;
		}
		const auto model = std::move(*maybe_model);
		std::println("Size: {}", model.size);
	}
	std::println();
}
