#ifndef B6EA0424_ACA3_474F_9663_68A01C516970
#define B6EA0424_ACA3_474F_9663_68A01C516970

#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/fixedrateinstruments.hpp>

namespace QuantLibParser {
    using namespace Atlas;

    template <>
    void Schema<FixedCustomStructure>::initSchema() {
        json base = R"({
            "title": "Fixed Bullet Instrument Schema",
            "properties": {},
            "required": ["PAYMENTSCHEDULE", "RATE"]
        })"_json;

        json paymentSchedule = R"({
            "title": "Payment Schedule",
            "type": "array",
            "items": {
                "type": "object",
                "properties": {
                    "AMOUNT": {
                        "type": "number",
                        "minimum": 0
                    },
                },
                "required": ["DATE", "AMOUNT"]
            }
        })"_json;

        paymentSchedule["items"]["properties"]["DATE"] = dateSchema;
        base["properties"]["PAYMENTSCHEDULE"]          = paymentScheduleSchema;
        base["properties"]["AMOUNT"]                   = amountSchema;
        base["properties"]["RATE"]                     = baseRateSchema;

        mySchema_ = base;
    };

    template <>
    void Schema<FixedCustomStructure>::initDefaultValues(){};
}  // namespace QuantLibParser

#endif /* B6EA0424_ACA3_474F_9663_68A01C516970 */
