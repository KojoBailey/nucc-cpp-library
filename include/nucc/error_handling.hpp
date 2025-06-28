#pragma once

#include <iostream>
#include <format>
#include <functional>

namespace nucc {

enum class Status_Code {
    OK = 0,
    FILE_NULL,
    FILE_MAGIC,
    VERSION,
    POINTER_NULL,
    JSON_NULL,
    JSON_MISSING_FIELD,
    JSON_VALUE
};

class Error {
public:
    Error(Status_Code in_code, std::string in_specific, std::string in_suggestion) {
        code_store = in_code;
        specific_store = in_specific;
        suggestion_store = in_suggestion;
    }

    Status_Code code() {
        return code_store;
    }
    int number() {
        return (int)code_store;
    }
    std::string generic() {
        switch (code_store) {
            case Status_Code::FILE_NULL             : return "Attempted to load file with null data.";
            case Status_Code::FILE_MAGIC            : return "File magic / signature does not match expected value.";
            case Status_Code::VERSION               : return "Detected version does not match what is required.";
            case Status_Code::POINTER_NULL          : return "Received null pointer/address, but expected data.";
            case Status_Code::JSON_NULL             : return "JSON data is null.";
            case Status_Code::JSON_MISSING_FIELD    : return "JSON field not defined.";
            case Status_Code::JSON_VALUE            : return "Provided JSON value is invalid.";
        }
        return "Unknown error code.";
    }
    std::string specific() {
        return specific_store;
    }
    std::string suggestion() {
        return suggestion_store;
    }

    static void print(Error error) {
        std::cout << std::format("NUCC++ Error Code: {:03} - {}\n{}\nSuggestion: {}\n",
            error.number(), error.generic(), error.specific(), error.suggestion());
    }

private:
    Status_Code code_store;
    std::string specific_store;
    std::string suggestion_store;
};

} // namespace nucc