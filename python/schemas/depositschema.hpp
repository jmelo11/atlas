#ifndef AC7D969B_F3AA_445C_9566_77D45ED116A4
#define AC7D969B_F3AA_445C_9566_77D45ED116A4

#include <qlp/parser.hpp>
#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/fixedrate/deposit.hpp>

using namespace Atlas;

namespace QuantLibParser {

    template <>
    void Schema<Deposit>::initSchema() {
        json base = R"({
            "title": "Deposit Instrument Schema",
            "properties": {},
            "required": ["STARTDATE", "ENDDATE", "RATE"]
        })"_json;

        base["properties"]["STARTDATE"] = dateSchema;
        base["properties"]["ENDDATE"]   = dateSchema;
        base["properties"]["NOTIONAL"]  = faceAmountSchema;
        base["properties"]["RATE"]      = baseRateSchema;

        mySchema_ = base;
    };

    template <>
    void Schema<Deposit>::initDefaultValues() {
        myDefaultValues_["NOTIONAL"] = 100;
    };

    template <>
    std::optional<Deposit> Schema<Deposit>::makeObj(const json& params) {
        auto f = [&]() {
            json data                = setDefaultValues(params);
            QuantLib::Date startDate = parse<QuantLib::Date>(data.at("STARTDATE"));
            QuantLib::Date endDate   = parse<QuantLib::Date>(data.at("ENDDATE"));

            const json& rateParams = data.at("RATE");
            double r               = rateParams.at("VALUE");

            QuantLib::DayCounter dayCounter   = parse<QuantLib::DayCounter>(rateParams.at("DAYCOUNTER"));
            QuantLib::Compounding compounding = parse<QuantLib::Compounding>(rateParams.at("COMPOUNDING"));
            QuantLib::Frequency frequency     = parse<QuantLib::Frequency>(rateParams.at("FREQUENCY"));
            QuantLib::InterestRate rate(r, dayCounter, compounding, frequency);

            double notional = data.at("NOTIONAL");

            return std::make_optional<Deposit>(startDate, endDate, notional, rate);
        };
        return isValid(params) ? f() : std::nullopt;
    }
}  // namespace QuantLibParser

#endif /* AC7D969B_F3AA_445C_9566_77D45ED116A4 */
