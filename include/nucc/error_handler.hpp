#ifndef KOJO_NUCC_ERROR_HANDLER
#define KOJO_NUCC_ERROR_HANDLER

#include <iostream>
#include <format>

namespace nucc {

enum class status_code {
    ok = 0,
    null_file,
    file_magic,
    version,
    null_pointer,
};

class error {
public:
    error(status_code _code, std::string _specific, std::string _suggestion) 
    : m_code(_code), m_specific(_specific), m_suggestion(_suggestion) {}

    status_code code() { return m_code; }
    int number() { return (int)m_code; }
    std::string generic() {
        switch (m_code) {
            case status_code::null_file             : return "Attempted to load file with null data.";
            case status_code::file_magic            : return "File magic / signature does not match expected value.";
            case status_code::version               : return "Detected version does not match what is required.";
            case status_code::null_pointer          : return "Received null pointer/address, but expected data.";
        }
        return "Unknown error code.";
    }
    std::string specific() {
        return m_specific;
    }
    std::string suggestion() {
        return m_suggestion;
    }

    static void print(error err) {
        std::cout << std::format("NUCC++ Error Code: {:03} - {}\n{}\nSuggestion: {}\n",
            err.number(), err.generic(), err.specific(), err.suggestion());
    }
    static void print(status_code _code, std::string _specific, std::string _suggestion) {
        error err{_code, _specific, _suggestion};
        print(err);
    }

private:
    status_code m_code;
    std::string m_specific;
    std::string m_suggestion;
};

}

#endif // KOJO_NUCC_ERROR_HANDLER