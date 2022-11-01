#include <atlaspython/schemas/instrumentschemas.hpp>

namespace AtlasPython {

    inline json baseRateSchema() {
        json schema = R"({
            "title": "Base Rate Schema",
            "properties": {
                "VALUE": {
                    "type": "number",
                    "description": "Base rate"
                }
            },
            "required": ["VALUE", "DAYCOUNTER", "COMPOUNDING", "FREQUENCY"]
        })"_json;

        schema["properties"]["DAYCOUNTER"]  = dayCounterSchema;
        schema["properties"]["COMPOUNDING"] = compoundingSchema;
        schema["properties"]["FREQUENCY"]   = frequencySchema;

        return schema;
    }

    // FixedBullet
    template <>
    void QuantLibParser::Schema<FixedBullet>::initSchema() {
        json base = R"({
            "title": "Fixed Bullet Instrument Schema",
            "properties": {},
            "required": ["STARTDATE", "ENDDATE", "PAYMENTFREQUENCY", "AMOUNT", "RATE"]
        })"_json;

        base["properties"]["STARTDATE"]        = dateSchema;
        base["properties"]["ENDDATE"]          = dateSchema;
        base["properties"]["PAYMENTFREQUENCY"] = frequencySchema;
        base["properties"]["AMOUNT"]           = amountSchema;
        base["properties"]["RATE"]             = baseRateSchema();

        mySchema_ = base;
    };

    template <>
    void QuantLibParser::Schema<FixedBullet>::initDefaultValues(){};

    // DAP
    template <>
    void QuantLibParser::Schema<DAP>::initSchema() {
        json base = R"({
            "title": "Deposit Instrument Schema",
            "properties": {},
            "required": ["STARTDATE", "ENDDATE", "PAYMENTFREQUENCY", "AMOUNT", "RATE"]
        })"_json;

        base["properties"]["STARTDATE"]        = dateSchema;
        base["properties"]["ENDDATE"]          = dateSchema;
        base["properties"]["PAYMENTFREQUENCY"] = frequencySchema;
        base["properties"]["AMOUNT"]           = amountSchema;
        base["properties"]["RATE"]             = baseRateSchema();

        mySchema_ = base;
    };

    template <>
    void QuantLibParser::Schema<FixedBullet>::initDefaultValues(){};

    template class QuantLibParser::Schema<FixedBullet>;
}  // namespace AtlasPython