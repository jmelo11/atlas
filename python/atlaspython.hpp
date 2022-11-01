#ifndef FE2F213A_1675_4B40_B0AE_5F51E9404366
#define FE2F213A_1675_4B40_B0AE_5F51E9404366

#include <atlaspython/schemas/instrumentschemas.hpp>
#include <nlohmann/json.hpp>

#define ADDINDEX(myMap, name) myMap[#name] = name()

namespace AltasPython {

    using IndexMap = std::unordered_map<std::string, RateIndex>;
    static void createIndeces(IndexMap& map) {
        ADDINDEX(map, LIBOR1M);
        ADDINDEX(map, LIBOR3M);
        ADDINDEX(map, LIBOR6M);
        ADDINDEX(map, LIBOR12M);
        ADDINDEX(map, ICP);
    }

    template <typename T>
    static T buildProd(const json& params);

    template <>
    static FixedBullet buildProd(const json& params) {
        CHEKNPARSE(params, Date, STARTDATE);
        CHEKNPARSE(params, Date, ENDDATE);
        CHEKNPARSE(params, Frequency, PAYMENTFREQUENCY);

        checkFor(params, "AMOUNT");
        double AMOUNT = params.at("AMOUNT");

        checkFor(params, "RATE");
        const json& rateParams = params.at("RATE");

        SETVAR(rateParams, double, VALUE, 0.01);
        CHEKNPARSE(rateParams, DayCounter, DAYCOUNTER);
        CHEKNPARSE(rateParams, Frequency, FREQUENCY);
        CHEKNPARSE(rateParams, Compounding, COMPOUNDING);
        InterestRate RATE(VALUE, DAYCOUNTER, COMPOUNDING, FREQUENCY);
        MetaData METADATA;
        return FixedBullet(STARTDATE, ENDDATE, PAYMENTFREQUENCY, AMOUNT, RATE, METADATA);
    }

    template <>
    static EqualPayment buildProd(const json& params) {
        CHEKNPARSE(params, Date, STARTDATE);
        CHEKNPARSE(params, Date, ENDDATE);
        CHEKNPARSE(params, Frequency, PAYMENTFREQUENCY);

        checkFor(params, "AMOUNT");
        double AMOUNT = params.at("AMOUNT");

        checkFor(params, "RATE");
        const json& rateParams = params.at("RATE");

        SETVAR(rateParams, double, VALUE, 0.01);
        CHEKNPARSE(rateParams, DayCounter, DAYCOUNTER);
        CHEKNPARSE(rateParams, Frequency, FREQUENCY);
        CHEKNPARSE(rateParams, Compounding, COMPOUNDING);
        InterestRate RATE(VALUE, DAYCOUNTER, COMPOUNDING, FREQUENCY);
        MetaData METADATA;
        return EqualPayment(STARTDATE, ENDDATE, PAYMENTFREQUENCY, AMOUNT, RATE, METADATA);
    }

    template <>
    static FloatingBullet buildProd(const json& params) {
        CHEKNPARSE(params, Date, STARTDATE);
        CHEKNPARSE(params, Date, ENDDATE);
        CHEKNPARSE(params, Frequency, PAYMENTFREQUENCY);

        checkFor(params, "AMOUNT");
        double AMOUNT = params.at("AMOUNT");

        checkFor(params, "SPREAD");
        double SPREAD = params.at("SPREAD");
        checkFor(params, "INDEX");
        IndexMap indices;
        createIndeces(indices);
        if (indices.find(params.at("INDEX")) == indices.end())
            throw std::runtime_error("Index " + (std::string)params.at("INDEX") + " not supported.");

        const std::string& indexName = params.at("INDEX");
        RateIndex index              = indices.at(indexName);
        MetaData METADATA("undefined", index);
        return FloatingBullet(STARTDATE, ENDDATE, AMOUNT, PAYMENTFREQUENCY, SPREAD, METADATA);
    }

    template <>
    static FloatingCustomStructure buildProd(const json& params) {
        checkFor(params, "DATES");
        checkFor(params, "REDEMPTIONS");

        const json& dates       = params.at("DATES");
        const json& redemptions = params.at("REDEMPTIONS");
        if (dates.size() - 1 != redemptions.size()) throw std::runtime_error("Dates and redemptions must have the same size");

        std::vector<Date> datesVec;
        for (auto it = dates.begin(); it != dates.end(); ++it) { datesVec.push_back(parseDate((std::string)*it, DateFormat::MIXED)); }
        std::vector<double> redemptionsVec;
        for (auto it = redemptions.begin(); it != redemptions.end(); ++it) { redemptionsVec.push_back((double)*it); }

        checkFor(params, "SPREAD");
        double SPREAD = params.at("SPREAD");

        checkFor(params, "INDEX");
        IndexMap indices;
        createIndeces(indices);
        if (indices.find(params.at("INDEX")) == indices.end())
            throw std::runtime_error("Index " + (std::string)params.at("INDEX") + " not supported.");

        const std::string& indexName = params.at("INDEX");
        RateIndex index              = indices.at(indexName);
        MetaData METADATA("undefined", index);
        return FloatingCustomStructure(datesVec, redemptionsVec, SPREAD, METADATA);
    }

    template <>
    static FixedCustomStructure buildProd(const json& params) {
        IndexMap indices;
        createIndeces(indices);

        checkFor(params, "DATES");
        checkFor(params, "REDEMPTIONS");

        const json& dates       = params.at("DATES");
        const json& redemptions = params.at("REDEMPTIONS");
        if (dates.size() - 1 != redemptions.size()) throw std::runtime_error("Dates and redemptions must have the same size");

        std::vector<Date> datesVec;
        for (auto it = dates.begin(); it != dates.end(); ++it) { datesVec.push_back(parseDate((std::string)*it, DateFormat::MIXED)); }
        std::vector<double> redemptionsVec;
        for (auto it = redemptions.begin(); it != redemptions.end(); ++it) { redemptionsVec.push_back((double)*it); }

        checkFor(params, "RATE");
        const json& rateParams = params.at("RATE");

        SETVAR(rateParams, double, VALUE, 0.01);
        CHEKNPARSE(rateParams, DayCounter, DAYCOUNTER);
        CHEKNPARSE(rateParams, Frequency, FREQUENCY);
        CHEKNPARSE(rateParams, Compounding, COMPOUNDING);
        InterestRate RATE(VALUE, DAYCOUNTER, COMPOUNDING, FREQUENCY);
        MetaData METADATA;
        return FixedCustomStructure(datesVec, redemptionsVec, RATE, METADATA);
    }

    static std::unique_ptr<YieldCurve> buildDiscountCurve(const json& params) {
        checkFor(params, "NODES");
        const json& nodes = params.at("NODES");
        std::vector<Date> dates;
        std::vector<double> dfs;
        for (auto& [k, v] : nodes.items()) {
            dates.push_back(parseDate(k, DateFormat::MIXED));
            dfs.push_back(v);
        }
        return std::unique_ptr<YieldCurve>(new DiscountCurve<LogLinear>(dates, dfs));
    };

    static std::unique_ptr<ZeroRateCurve<Linear>> buildDummyCurve(const Date& refDate) {
        return std::make_unique<ZeroRateCurve<Linear>>(buildFlatCurve<Linear>(refDate, 0.03));
    };

    template <typename T, typename CalcType>
    struct pricer;

    template <typename T, typename CalcType>
    struct pricer {
        static double defineAndCalculate(const json& params);
    };

    template <typename T, typename CalcType>
    requires std::derived_from<T, FixedRateInstrument>
    struct pricer<T, CalcType> {
        static double defineAndCalculate(const json& params) {
            // builds prod
            T prod = buildProd<T>(params);
            // predefine curve maps
            std::unordered_map<std::string, std::unique_ptr<YieldCurve>> discountCurves;
            std::unordered_map<std::string, std::unique_ptr<YieldCurve>> forecastCurves;

            // check discount curve and define
            checkFor(params, "DISCOUNTCURVE");
            const json& discountParams                = params.at("DISCOUNTCURVE");
            std::unique_ptr<YieldCurve> discountCurve = buildDiscountCurve(discountParams);
            discountCurves["undefined"]               = std::move(discountCurve);

            // index discounts
            DFIndexer dfIndexer;
            prod.accept(dfIndexer);
            MarketRequest request;
            dfIndexer.setRequest(request);

            StaticCurveModel model(request, discountCurves, forecastCurves);
            CHEKNPARSE(params, Date, REFDATE);
            std::vector<Date> evalDate(1, REFDATE);
            std::vector<MarketData> marketData;
            model.simulate(evalDate, marketData);

            CalcType func(marketData[0]);
            prod.accept(func);
            return func.result();
        }
    };

    template <typename T, typename CalcType>
    requires std::derived_from<T, FloatingRateInstrument>
    struct pricer<T, CalcType> {
        static double defineAndCalculate(const json& params) {
            // builds prod
            T prod = buildProd<T>(params);

            // predefine curve maps
            std::unordered_map<std::string, std::unique_ptr<YieldCurve>> discountCurves;
            std::unordered_map<std::string, std::unique_ptr<YieldCurve>> forecastCurves;

            // check discount curve and define
            checkFor(params, "DISCOUNTCURVE");
            const json& discountParams                = params.at("DISCOUNTCURVE");
            std::unique_ptr<YieldCurve> discountCurve = buildDiscountCurve(discountParams);
            discountCurves["undefined"]               = std::move(discountCurve);

            // index discounts
            DFIndexer dfIndexer;
            prod.accept(dfIndexer);
            MarketRequest request;
            dfIndexer.setRequest(request);

            // check if floating and define
            checkFor(params, "FORECASTCURVE");
            const json& forecastParams                = params.at("FORECASTCURVE");
            std::unique_ptr<YieldCurve> forecastCurve = buildDiscountCurve(forecastParams);
            forecastCurves["undefined"]               = std::move(forecastCurve);

            ForwardRatendexer fwdIndexer;
            prod.accept(fwdIndexer);
            fwdIndexer.setRequest(request);

            StaticCurveModel model(request, discountCurves, forecastCurves);
            CHEKNPARSE(params, Date, REFDATE);
            std::vector<Date> evalDate(1, REFDATE);
            std::vector<MarketData> marketData;
            model.simulate(evalDate, marketData);

            InterestCalculator interestCacl(marketData[0]);
            prod.accept(interestCacl);

            CalcType func(marketData[0]);
            prod.accept(func);
            return func.result();
        }
    };
}  // namespace AltasPython

#endif /* FE2F213A_1675_4B40_B0AE_5F51E9404366 */
