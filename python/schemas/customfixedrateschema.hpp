#ifndef B55BD725_E680_45C6_8944_015D84AA0F3C
#define B55BD725_E680_45C6_8944_015D84AA0F3C

#include <qlp/parser.hpp>
#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/fixedrate/customfixedrateproduct.hpp>

using namespace Atlas;

namespace QuantLibParser {

    template <>
    void Schema<CustomFixedRateProduct>::initSchema() {
        json base = R"({
            "title": "Custom Profile Instrument Schema",
            "properties": {},
            "required": ["STARTDATE", "REDEMPTIONS", "RATE"]
        })"_json;

        base["properties"]["REDEMPTIONS"] = R"({
            "type": "array",
            "items": {
                "type":"object",
                "properties": {
                    "VALUE" : {"type": "number"}    
                }
            },
            "minItems":1
        })"_json;

        base["properties"]["STARTDATE"]                                  = dateSchema;
        base["properties"]["REDEMPTIONS"]["items"]["properties"]["DATE"] = dateSchema;
        base["properties"]["RATE"]                                       = baseRateSchema;

        mySchema_ = base;
    };

    template <>
    void Schema<CustomFixedRateProduct>::initDefaultValues(){};

    template <>
    template <>
    CustomFixedRateProduct Schema<CustomFixedRateProduct>::makeObj(const json& params) {
        json data = setDefaultValues(params);
        validate(data);

        QuantLib::Date startDate          = parse<QuantLib::Date>(data.at("STARTDATE"));
        std::vector<QuantLib::Date> dates = {startDate};
        std::vector<double> redemptions;
        for (const auto& pair : data.at("REDEMPTIONS")) {
            dates.push_back(parse<QuantLib::Date>(pair.at("DATE")));
            redemptions.push_back(pair.at("VALUE"));
        }

        const json& rateParams = data.at("RATE");
        double r               = rateParams.at("VALUE");

        QuantLib::DayCounter dayCounter   = parse<QuantLib::DayCounter>(rateParams.at("DAYCOUNTER"));
        QuantLib::Compounding compounding = parse<QuantLib::Compounding>(rateParams.at("COMPOUNDING"));
        QuantLib::Frequency frequency     = parse<QuantLib::Frequency>(rateParams.at("FREQUENCY"));
        QuantLib::InterestRate rate(r, dayCounter, compounding, frequency);

        return CustomFixedRateProduct(dates, redemptions, rate);
    };
}  // namespace QuantLibParser

#endif /* B55BD725_E680_45C6_8944_015D84AA0F3C */
