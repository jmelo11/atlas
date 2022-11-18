#ifndef A396A604_3AC7_4553_AB8C_69317DD21C43
#define A396A604_3AC7_4553_AB8C_69317DD21C43

#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/fixedrate/fixedbulletproduct.hpp>

namespace AtlasPython {

    using namespace Atlas;

    namespace QuantLibParser {

        template <>
        void Schema<FixedBulletProduct>::initSchema() {
            json base = R"({
            "title": "Fixed Bullet Instrument Schema",
            "properties": {},
            "required": ["startDate", "endDate", "paymentFrequency", "rate"]
        })"_json;

            base["properties"]["startDate"]        = dateSchema;
            base["properties"]["endDate"]          = dateSchema;
            base["properties"]["paymentFrequency"] = frequencySchema;
            base["properties"]["notional"]         = amountSchema;
            base["properties"]["rate"]             = baseRateSchema;

            mySchema_ = base;
        };

        template <>
        void Schema<FixedBullet>::initDefaultValues() {
            myDefaultValues_["notional"] = 100;
        };

        template <>
        auto Schema<FixedBulletProduct>::makeObj(const json& params) {
            auto f = [&](const json& params) {
                json data                            = setDefaultValues(params);
                QuantLib::Date startDate             = parser<QuantLib::Date> parse(data.at("STARTDATE"));
                QuantLib::Date endDate               = parser<QuantLib::Date> parse(data.at("ENDDATE"));
                QuantLib::Frequency paymentFrequency = parser<QuantLib::Frequency> parse(rateParams.at("PAYMENTFREQUENCY"));

                const json& rateParams = data.at("RATE");
                double r               = rateParams.at("VALUE");

                QuantLib::DayCounter dayCounter   = parser<QuantLib::DayCounter> parse(rateParams.at("DAYCOUNTER"));
                QuantLib::Compounding compounding = parser<QuantLib::Compounding> parse(rateParams.at("COMPOUNDING"));
                QuantLib::Frequency frequency     = parser<QuantLib::Frequency> parse(rateParams.at("FREQUENCY"));
                QuantLib::InterestRate rate(r, dayCounter, compounding, frequency);

                double notional = data.at("NOTIONAL");
                return std::make_optional<FixedBulletProduct>(startDate, endDate, paymentFrequency, notional, rate);
            };
            return isValid(params) ? f(params) : std::nullopt;
        }
    }  // namespace QuantLibParser

}  // namespace AtlasPython

#endif /* A396A604_3AC7_4553_AB8C_69317DD21C43 */
