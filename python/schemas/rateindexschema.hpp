#ifndef D8690143_6F6E_4726_B6E4_A491057A01CF
#define D8690143_6F6E_4726_B6E4_A491057A01CF

#include <qlp/parser.hpp>
#include <qlp/schemas/commonschemas.hpp>
#include <atlas/curves/rateindex.hpp>
#include <unordered_map>

using namespace Atlas;

namespace QuantLibParser {

    static std::unordered_map<std::string, RateIndex> IndexMap = {
        {"LIBOR1M", LIBOR1M()}, {"LIBOR3M", LIBOR3M()}, {"LIBOR6M", LIBOR6M()}, {"LIBOR12M", LIBOR12M()}, {"ICP", ICP()},
    };

    template <>
    void Schema<RateIndex>::initSchema() {
        json base = R"({
                "title": "Rate Index Schema",
                "properties": {
                    "NAME": {
                        "type": "string"
                    }
                },
                "required": ["NAME"]
            })"_json;

        base["properties"]["NAME"]            = curveNameSchema;
        base["properties"]["DAYCOUNTER"]      = dayCounterSchema;
        base["properties"]["FIXINGFREQUENCY"] = frequencySchema;
        base["properties"]["RATEFREQUENCY"]   = frequencySchema;
        base["properties"]["RATECOMPOUNDING"] = compoundingSchema;

        mySchema_ = base;
    };

    template <>
    void Schema<RateIndex>::initDefaultValues() {
        myDefaultValues_["FIXINGFREQUENCY"] = "SEMIANNUAL";
        myDefaultValues_["RATEFREQUENCY"]   = "ANNUAL";
        myDefaultValues_["RATECOMPOUNDING"] = "SIMPLE";
        myDefaultValues_["DAYCOUNTER"]      = "ACT360";
    };

    template <>
    template <>
    RateIndex Schema<RateIndex>::makeObj(const json& params) {
        json data = setDefaultValues(params);
        validate(data);
        if (IndexMap.find(data.at("NAME")) != IndexMap.end()) { return IndexMap[data.at("NAME")]; }
        const std::string& name               = params.at("NAME");
        QuantLib::DayCounter dayCounter       = parse<QuantLib::DayCounter>(data.at("DAYCOUNTER"));
        QuantLib::Frequency fixingFrequency   = parse<QuantLib::Frequency>(data.at("FIXINGFREQUENCY"));
        QuantLib::Frequency rateFrequency     = parse<QuantLib::Frequency>(data.at("RATEFREQUENCY"));
        QuantLib::Compounding rateCompounding = parse<QuantLib::Compounding>(data.at("RATECOMPOUNDING"));
        return RateIndex(name, dayCounter, fixingFrequency, rateFrequency, rateCompounding);
    };

}  // namespace QuantLibParser

#endif /* D8690143_6F6E_4726_B6E4_A491057A01CF */
