#include <xfbin/xfbin.hpp>

#include <print>
#include <iostream>

using namespace kojo;

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::println(std::cerr, "Too few arguments.");
		return 1;
	}

	auto maybe_xfbin = Xfbin::from(argv[1]);
	if (!maybe_xfbin) {
		std::println("Error: {}", maybe_xfbin.error().to_string());
		return 1;
	}
	Xfbin xfbin = *maybe_xfbin;

	std::println("{}", xfbin.get_version());
}
