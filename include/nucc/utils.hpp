#ifndef KOJO_NUCC_UTILS
#define KOJO_NUCC_UTILS

#include <algorithm>
#include <string>

inline std::string to_lowercase(std::string input) {
    std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return input;
}

#endif // KOJO_NUCC_UTILS