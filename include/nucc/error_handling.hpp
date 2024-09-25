#pragma once

#include <iostream>
#include <format>
#include <functional>

namespace nucc {

enum class Status_Code {
    OK = 0,
    POINTER_NULL,
    JSON_NULL,
    JSON_MISSING_FIELD,
    JSON_VALUE
};

struct Error {
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

private:
    Status_Code code_store;
    std::string specific_store;
    std::string suggestion_store;
};

inline std::function<int (Error)> error_handler = [](Error e) {
    std::cout << std::format("Error Code: {:03} - {}\n{}\nSuggestion: {}\n", e.number(), e.generic(), e.specific(), e.suggestion());
    return e.number();
};

} // namespace nucc