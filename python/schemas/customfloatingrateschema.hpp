#ifndef FDAB57B8_658C_4205_9E0C_A0F60C0AEC99
#define FDAB57B8_658C_4205_9E0C_A0F60C0AEC99

#include "rateindexschema.hpp"
#include <qlp/parser.hpp>
#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateproduct.hpp>

using namespace Atlas;

namespace QuantLibParser {

    template <>
    void Schema<CustomFloatingRateProduct>::initSchema() {
        json base = R"({
            "title": "Custom Profile Instrument Schema",
            "properties": {},
            "required": ["STARTDATE", "REDEMPTIONS", "SPREAD", "INDEX"]
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
        base["properties"]["SPREAD"]                                     = priceSchema;

        Schema<RateIndex> indexSchema;
        base["properties"]["INDEX"] = indexSchema.schema();

        mySchema_ = base;
    };

    template <>
    void Schema<CustomFloatingRateProduct>::initDefaultValues(){};

    template <>
    template <>
    CustomFloatingRateProduct Schema<CustomFloatingRateProduct>::makeObj(const json& params) {
        json data = setDefaultValues(params);
        validate(data);

        QuantLib::Date startDate          = parse<QuantLib::Date>(data.at("STARTDATE"));
        std::vector<QuantLib::Date> dates = {startDate};
        std::vector<double> redemptions;
        for (const auto& pair : data.at("REDEMPTIONS")) {
            dates.push_back(parse<QuantLib::Date>(pair.at("DATE")));
            redemptions.push_back(pair.at("VALUE"));
        }

        double spread = data.at("SPREAD");

        Schema<RateIndex> indexSchema;
        auto index = indexSchema.makeObj(data.at("INDEX"));

        return CustomFloatingRateProduct(dates, redemptions, spread, index);
    };
}  // namespace QuantLibParser

#endif /* FDAB57B8_658C_4205_9E0C_A0F60C0AEC99 */
