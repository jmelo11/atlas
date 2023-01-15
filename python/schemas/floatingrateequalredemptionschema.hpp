#ifndef FF7B27CE_6FD7_42FD_B377_81B662B1D570
#define FF7B27CE_6FD7_42FD_B377_81B662B1D570

#include "rateindexschema.hpp"
#include <qlp/parser.hpp>
#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptionproduct.hpp>

using namespace Atlas;

namespace QuantLibParser {

    template <>
    void Schema<FloatingRateEqualRedemptionProduct>::initSchema() {
        json base = R"({
            "title": "Floating rate equal redemption product schema",
            "properties": {},
            "required": ["STARTDATE", "ENDDATE", "SPREAD", "INDEX"]
        })"_json;

        base["properties"]["STARTDATE"] = dateSchema;
        base["properties"]["ENDDATE"]   = dateSchema;
        base["properties"]["SPREAD"]    = priceSchema;
        base["properties"]["NOTIONAL"]  = faceAmountSchema;

        Schema<RateIndex> indexSchema;
        base["properties"]["INDEX"] = indexSchema.schema();

        mySchema_ = base;
    };

    template <>
    void Schema<FloatingRateEqualRedemptionProduct>::initDefaultValues() {
        myDefaultValues_["NOTIONAL"] = 100;
    };

    template <>
    template <>
    FloatingRateEqualRedemptionProduct Schema<FloatingRateEqualRedemptionProduct>::makeObj(const json& params) {
        json data = setDefaultValues(params);
        validate(data);
        QuantLib::Date startDate = parse<QuantLib::Date>(data.at("STARTDATE"));
        QuantLib::Date endDate   = parse<QuantLib::Date>(data.at("ENDDATE"));
        double spread            = data.at("SPREAD");
        double notional          = data.at("NOTIONAL");

        Schema<RateIndex> indexSchema;
        auto index = indexSchema.makeObj(data.at("INDEX"));
        return FloatingRateEqualRedemptionProduct(startDate, endDate, notional, spread, index);
    };

}  // namespace QuantLibParser

#endif /* FF7B27CE_6FD7_42FD_B377_81B662B1D570 */
