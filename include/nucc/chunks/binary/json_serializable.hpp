#ifndef KOJO_NUCC_JSON_SERIALIZABLE
#define KOJO_NUCC_JSON_SERIALIZABLE

#include <nlohmann/json.hpp>

namespace nucc {

class json_serializable {
public:
    virtual ~json_serializable() = default;

    json_serializable(const nlohmann::ordered_json& input) {
        read_json(input);
    }

    virtual void read_json(const nlohmann::ordered_json& input) = 0;
    virtual nlohmann::ordered_json write_json() = 0;
};

}

#endif // KOJO_NUCC_JSON_SERIALIZABLE