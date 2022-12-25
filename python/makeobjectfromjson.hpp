#ifndef A7732A1B_652A_459D_BD38_7519ADFA7C1E
#define A7732A1B_652A_459D_BD38_7519ADFA7C1E

#include <qlp/schemas/schema.hpp>

using json = nlohmann::json;

template <typename T>
inline T makeObjectFromJson(const json& params) {
    QuantLibParser::Schema<T> schema;
    schema.validate(params);
    json finalParams = schema.setDefaultValues(params);
    return schema.makeObj(finalParams);
};

#endif /* A7732A1B_652A_459D_BD38_7519ADFA7C1E */
