#include <xfbin/xfbin.hpp>
#include <xfbin/chunk_binary.hpp>

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

	const nucc::Chunk* chunk = xfbin.fetch_chunk({}, {}, "PlayerColorParam");
	if (chunk == nullptr) {
		std::println(std::cerr, "[ERROR] Could not find specified chunk.");
		return 1;
	}
	const auto maybe_binary = nucc::ChunkBinary::from(*chunk);
	if (!maybe_binary) {
		std::println(std::cerr, "[ERROR] {}", maybe_binary.error().to_string());
		return 1;
	}
	const auto binary = std::move(*maybe_binary);
	std::println("Size: {}", binary.size);
}
