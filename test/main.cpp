#include <xfbin/xfbin.hpp>

#include <print>
#include <iostream>

using namespace kojo::nucc;

int main(int argc, char* argv[])
{
	std::println("Welcome to the XFBIN lib testing ground!\n");

	if (argc < 2) {
		std::println(std::cerr, "Too few arguments.");
		return 1;
	}

	const auto maybe_xfbin = Xfbin::from(argv[1]);
	if (!maybe_xfbin) {
		std::println("Error {:03}: {}",
				maybe_xfbin.error().to_code(),
				maybe_xfbin.error().to_string()
		);
		return 1;
	}
	const Xfbin xfbin = *maybe_xfbin;

	std::println("Version: {}", xfbin.get_version());
	std::println();

	const auto pages = xfbin.get_pages();
	for (const auto page : pages) {
		std::println("== Page ==");
		std::println();
		for (const auto chunk : page.chunks) {
			std::println("Type: {}", chunk_type_to_string(chunk.type));
			std::println("Path: {}", chunk.path);
			std::println("Name: {}", chunk.name);
			std::println();
		}
	}
}
