#ifndef D15DC036_1DC7_46E3_81C5_EC9739FD7433
#define D15DC036_1DC7_46E3_81C5_EC9739FD7433

#include <qlp/parser.hpp>
#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/fixedrate/equalpaymentproduct.hpp>

using namespace Atlas;

namespace QuantLibParser {

    template <>
    void Schema<EqualPaymentProduct>::initSchema() {
        json base = R"({
            "title": "Equal Payment Instrument Schema",
            "properties": {},
            "required": ["STARTDATE", "ENDDATE", "PAYMENTFREQUENCY", "RATE" ]
        })"_json;

        base["properties"]["STARTDATE"]        = dateSchema;
        base["properties"]["ENDDATE"]          = dateSchema;
        base["properties"]["PAYMENTFREQUENCY"] = frequencySchema;
        base["properties"]["RATE"]             = baseRateSchema;
        base["properties"]["NOTIONAL"]         = faceAmountSchema;

        mySchema_ = base;
    };

    template <>
    void Schema<EqualPaymentProduct>::initDefaultValues() {
        myDefaultValues_["NOTIONAL"] = 100;
    };

    template <>
    std::optional<EqualPaymentProduct> Schema<EqualPaymentProduct>::makeObj(const json& params) {
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
            return std::make_optional<EqualPaymentProduct>(startDate, endDate, paymentFrequency, notional, rate);
        };
        return isValid(params) ? f() : std::nullopt;
    };
}  // namespace QuantLibParser

#endif /* D15DC036_1DC7_46E3_81C5_EC9739FD7433 */