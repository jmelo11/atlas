#ifndef A7732A1B_652A_459D_BD38_7519ADFA7C1E
#define A7732A1B_652A_459D_BD38_7519ADFA7C1E

#include <qlp/schemas/schema.hpp>
#include <nlohmann/json.hpp>

using namespace QuantLibParser;
using json = nlohmann::json;

namespace AtlasPython {

    template <typename T>
    inline T make(const json& params) {
        Schema<T> schema;
        auto result = schema.makeObj(params);
        if (result.has_value()) {
            return result.value();
        } else {
            throw std::runtime_error("Failed to make");
        }
    };

}  // namespace AtlasPython

#endif /* A7732A1B_652A_459D_BD38_7519ADFA7C1E */
