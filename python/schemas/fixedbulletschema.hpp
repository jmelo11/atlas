#ifndef A396A604_3AC7_4553_AB8C_69317DD21C43
#define A396A604_3AC7_4553_AB8C_69317DD21C43

#include <qlp/parser.hpp>
#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/fixedrate/fixedbulletproduct.hpp>

using namespace Atlas;

namespace QuantLibParser {

    template <>
    void Schema<FixedBulletProduct>::initSchema() {
        json base = R"({
            "title": "Fixed Bullet Instrument Schema",
            "properties": {},
            "required": ["STARTDATE", "ENDDATE", "PAYMENTFREQUENCY", "NOTIONAL"]
        })"_json;

        base["properties"]["STARTDATE"]        = dateSchema;
        base["properties"]["ENDDATE"]          = dateSchema;
        base["properties"]["PAYMENTFREQUENCY"] = frequencySchema;
        base["properties"]["NOTIONAL"]         = faceAmountSchema;
        base["properties"]["RATE"]             = baseRateSchema;

        mySchema_ = base;
    };

    template <>
    void Schema<FixedBulletProduct>::initDefaultValues() {
        myDefaultValues_["NOTIONAL"] = 100;
    };

    template <>
    std::optional<FixedBulletProduct> Schema<FixedBulletProduct>::makeObj(const json& params) {
        auto f = [&]() {
            json data                            = setDefaultValues(params);
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
            return std::make_optional<FixedBulletProduct>(startDate, endDate, paymentFrequency, notional, rate);
        };
        return isValid(params) ? f() : std::nullopt;
    }
}  // namespace QuantLibParser

#endif /* A396A604_3AC7_4553_AB8C_69317DD21C43 */
