#ifndef BB94E6AF_7041_4595_B941_117476A231A9
#define BB94E6AF_7041_4595_B941_117476A231A9

#include <qlp/parser.hpp>
#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/fixedrate/fixedrateequalredemptionproduct.hpp>

using namespace Atlas;

namespace QuantLibParser {

    template <>
    void Schema<FixedRateEqualRedemptionProduct>::initSchema() {
        json base = R"({
            "title": "Fixed rate equal redemption instrument schema",
            "properties": {},
            "required": ["STARTDATE", "ENDDATE", "PAYMENTFREQUENCY", "NOTIONAL", "RATE"]
        })"_json;

        base["properties"]["STARTDATE"]        = dateSchema;
        base["properties"]["ENDDATE"]          = dateSchema;
        base["properties"]["PAYMENTFREQUENCY"] = frequencySchema;
        base["properties"]["NOTIONAL"]         = faceAmountSchema;
        base["properties"]["RATE"]             = baseRateSchema;

        mySchema_ = base;
    };

    template <>
    void Schema<FixedRateEqualRedemptionProduct>::initDefaultValues() {
        myDefaultValues_["NOTIONAL"] = 100;
    };

    template <>
    template <>
    FixedRateEqualRedemptionProduct Schema<FixedRateEqualRedemptionProduct>::makeObj(const json& params) {
        json data = setDefaultValues(params);
        validate(data);
        QuantLib::Date startDate             = parse<QuantLib::Date>(data.at("STARTDATE"));
        QuantLib::Date endDate               = parse<QuantLib::Date>(data.at("ENDDATE"));
        QuantLib::Frequency paymentFrequency = parse<QuantLib::Frequency>(data.at("PAYMENTFREQUENCY"));

        const json& rateParams = data.at("RATE");
        double r               = rateParams.at("VALUE");

        QuantLib::DayCounter dayCounter   = parse<QuantLib::DayCounter>(rateParams.at("DAYCOUNTER"));
        QuantLib::Compounding compounding = parse<QuantLib::Compounding>(rateParams.at("COMPOUNDING"));
        QuantLib::Frequency frequency     = parse<QuantLib::Frequency>(rateParams.at("FREQUENCY"));
        QuantLib::InterestRate rate(r, dayCounter, compounding, frequency);

        double notional = data.at("NOTIONAL");
        return FixedRateEqualRedemptionProduct(startDate, endDate, paymentFrequency, notional, rate);
    };

}  // namespace QuantLibParser

#endif /* BB94E6AF_7041_4595_B941_117476A231A9 */
