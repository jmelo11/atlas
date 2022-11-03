#ifndef D15DC036_1DC7_46E3_81C5_EC9739FD7433
#define D15DC036_1DC7_46E3_81C5_EC9739FD7433

#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/fixedrateinstruments.hpp>

namespace QuantLibParser {
    using namespace Atlas;

    template <>
    void Schema<EqualPayment>::initSchema() {
        json base = R"({
            "title": "Equal Payment Instrument Schema",
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
    void Schema<EqualPayment>::initDefaultValues() {
        myDefaultValues_["AMOUNT"] = 100;
    };
}  // namespace QuantLibParser

#endif /* D15DC036_1DC7_46E3_81C5_EC9739FD7433 */
