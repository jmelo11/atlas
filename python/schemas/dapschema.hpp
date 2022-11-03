#ifndef AC7D969B_F3AA_445C_9566_77D45ED116A4
#define AC7D969B_F3AA_445C_9566_77D45ED116A4

#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/fixedrateinstruments.hpp>

namespace QuantLibParser {
    using namespace Atlas;

    template <>
    void Schema<DAP>::initSchema() {
        json base = R"({
            "title": "Deposit Instrument Schema",
            "properties": {},
            "required": ["STARTDATE", "ENDDATE", "RATE"]
        })"_json;

        base["properties"]["STARTDATE"] = dateSchema;
        base["properties"]["ENDDATE"]   = dateSchema;
        base["properties"]["AMOUNT"]    = amountSchema;
        base["properties"]["RATE"]      = baseRateSchema;

        mySchema_ = base;
    };

    template <>
    void Schema<DAP>::initDefaultValues() {
        myDefaultValues_["AMOUNT"] = 100;
    };
}  // namespace QuantLibParser

#endif /* AC7D969B_F3AA_445C_9566_77D45ED116A4 */
