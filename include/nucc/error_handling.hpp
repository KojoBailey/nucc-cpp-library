#pragma once

#include <iostream>
#include <format>
#include <functional>

namespace nucc {

enum class status_code {
    ok = 0,
    null_file,
    file_magic,
    version,
    null_pointer,
    null_json,
    json_missing_field,
    json_value
};

class error {
public:
    error(status_code in_code, std::string in_specific, std::string in_suggestion) {
        code_store = in_code;
        specific_store = in_specific;
        suggestion_store = in_suggestion;
    }

    status_code code() {
        return code_store;
    }
    int number() {
        return (int)code_store;
    }
    std::string generic() {
        switch (code_store) {
            case status_code::null_file             : return "Attempted to load file with null data.";
            case status_code::file_magic            : return "File magic / signature does not match expected value.";
            case status_code::version               : return "Detected version does not match what is required.";
            case status_code::null_pointer          : return "Received null pointer/address, but expected data.";
            case status_code::null_json             : return "JSON data is null.";
            case status_code::json_missing_field    : return "JSON field not defined.";
            case status_code::json_value            : return "Provided JSON value is invalid.";
        }
        return "Unknown error code.";
    }
    std::string specific() {
        return specific_store;
    }
    std::string suggestion() {
        return suggestion_store;
    }

    static void print(error err) {
        std::cout << std::format("NUCC++ Error Code: {:03} - {}\n{}\nSuggestion: {}\n",
            err.number(), err.generic(), err.specific(), err.suggestion());
    }

private:
    status_code code_store;
    std::string specific_store;
    std::string suggestion_store;
};

} // namespace nucc