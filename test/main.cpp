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

	const auto types = xfbin.get_types();
	std::println("Types:");
	for (auto type : types) {
		std::println("{}", type);
	}
	std::println();

	const auto paths = xfbin.get_paths();
	std::println("Paths:");
	for (auto path : paths) {
		std::println("{}", path);
	}
	std::println();

	const auto names = xfbin.get_names();
	std::println("Names:");
	for (auto name : names) {
		std::println("{}", name);
	}
}
