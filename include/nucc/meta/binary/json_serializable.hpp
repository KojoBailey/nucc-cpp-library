#ifndef KOJO_NUCC_JSON_SERIALIZABLE
#define KOJO_NUCC_JSON_SERIALIZABLE

#include <nlohmann/json.hpp>

namespace nucc {

template<typename T>
class json_serializer {
public:
    static T read(const nlohmann::ordered_json&);
    static nlohmann::ordered_json write(const T&);
};

}

#endif // KOJO_NUCC_JSON_SERIALIZABLE