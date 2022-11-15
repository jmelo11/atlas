#ifndef A396A604_3AC7_4553_AB8C_69317DD21C43
#define A396A604_3AC7_4553_AB8C_69317DD21C43

#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/fixedrateinstruments.hpp>

namespace QuantLibParser {
    using namespace Atlas;

    template <>
    void Schema<FixedBullet>::initSchema() {
        json base = R"({
            "title": "Fixed Bullet Instrument Schema",
            "properties": {},
            "required": ["STARTDATE", "ENDDATE", "PAYMENTFREQUENCY", "RATE"]
        })"_json;

        base["properties"]["STARTDATE"]        = dateSchema;
        base["properties"]["ENDDATE"]          = dateSchema;
        base["properties"]["PAYMENTFREQUENCY"] = frequencySchema;
        base["properties"]["AMOUNT"]           = amountSchema;
        base["properties"]["RATE"]             = baseRateSchema;

        mySchema_ = base;
    };

    template <>
    void Schema<FixedBullet>::initDefaultValues() {
        myDefaultValues_["AMOUNT"] = 100;
    };
}  // namespace QuantLibParser

#endif /* A396A604_3AC7_4553_AB8C_69317DD21C43 */
