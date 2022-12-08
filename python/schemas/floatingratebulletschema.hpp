#ifndef DFD30E0D_2007_447D_86BA_E5673BA94852
#define DFD30E0D_2007_447D_86BA_E5673BA94852

#include "rateindexschema.hpp"
#include <qlp/parser.hpp>
#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletproduct.hpp>

using namespace Atlas;

namespace QuantLibParser {

    template <>
    void Schema<FloatingRateBulletProduct>::initSchema() {
        json base = R"({
            "title": "Deposit Instrument Schema",
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
    void Schema<FloatingRateBulletProduct>::initDefaultValues() {
        myDefaultValues_["NOTIONAL"] = 100;
    };

    template <>
    template <>
    FloatingRateBulletProduct Schema<FloatingRateBulletProduct>::makeObj(const json& params) {
        json data = setDefaultValues(params);
        validate(data);
        QuantLib::Date startDate = parse<QuantLib::Date>(data.at("STARTDATE"));
        QuantLib::Date endDate   = parse<QuantLib::Date>(data.at("ENDDATE"));
        double spread            = data.at("SPREAD");
        double notional          = data.at("NOTIONAL");

        Schema<RateIndex> indexSchema;
        auto index = indexSchema.makeObj(data.at("INDEX"));
        return FloatingRateBulletProduct(startDate, endDate, notional, spread, index);
    };

}  // namespace QuantLibParser

#endif /* DFD30E0D_2007_447D_86BA_E5673BA94852 */
