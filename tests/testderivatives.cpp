#include "commonvars.hpp"
#include "pch.hpp"
#include <atlas/curves/curve.hpp>
#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/fixfloatswap.hpp>
#include <atlas/model/staticcurvemodel.hpp>
#include <atlas/visitors/calculators.hpp>
#include <atlas/visitors/indexers.hpp>

TEST(TestVanillaSwapConsistency, TestDerivatives) {
    Atlas::FixFloatSwap swap(Atlas::FixFloatSwap::Side::Payer, commonVars.startDate, commonVars.endDate, commonVars.rate, commonVars.paymentFreq,
                             commonVars.paymentFreq, commonVars.amount, commonVars.spread, commonVars.metaData);

    Atlas::MarketData marketData;

    Atlas::InterestCalculator calc(marketData);
    calc.visit(swap);

    Date lastDate = commonVars.startDate;
    std::vector<double> fixCoupons;
    for (const auto& d : commonVars.schedule.dates()) {
        double yf = commonVars.dayCounter.yearFraction(lastDate, d);
        if (yf > 0.0 + QL_EPSILON) { fixCoupons.push_back(commonVars.amount * (commonVars.rate.compoundFactor(yf) - 1)); }
        lastDate = d;
    }
    // fix leg
    const auto& fixInterest = swap.legs()[0].interest();
    EXPECT_EQ(fixInterest.size(), fixCoupons.size());
    for (size_t i = 0; i < fixCoupons.size(); i++) { EXPECT_FLOAT_EQ(fixInterest.at(i), fixCoupons.at(i)); }

    std::unique_ptr<Atlas::ZeroRateCurve<Cubic>> forecastCurve =
        std::make_unique<Atlas::ZeroRateCurve<Cubic>>(Atlas::buildFlatCurve<Cubic>(commonVars.startDate, commonVars.curveRate));

    std::unordered_map<std::string, std::unique_ptr<Atlas::YieldCurve>> discountCurves;
    std::unordered_map<std::string, std::unique_ptr<Atlas::YieldCurve>> forecastCurves;

    forecastCurves["undefined"] = std::move(forecastCurve);

    Atlas::ForwardRatendexer indexer;
    indexer.visit(swap);
    Atlas::MarketRequest request;
    indexer.setRequest(request);

    Atlas::StaticCurveModel model(request, discountCurves, forecastCurves);
    std::vector<Date> evalDates(1, Date(1, Month::August, 2022));
    std::vector<Atlas::MarketData> results;
    model.simulate(evalDates, results);

    Atlas::InterestCalculator caculator(results[0]);
    caculator.visit(swap);

    std::vector<double> floatCoupons;
    for (const auto& d : commonVars.schedule.dates()) {
        double yf = commonVars.dayCounter.yearFraction(lastDate, d);
        if (yf > 0.0 + QL_EPSILON) {
            double r = forecastCurves["undefined"]->forwardRate(lastDate, d);
            InterestRate rate(r, commonVars.dayCounter, Compounding::Simple, Frequency::Annual);
            floatCoupons.push_back(commonVars.amount * (rate.compoundFactor(yf) - 1));
        }
        lastDate = d;
    }
    const auto& floatInterest = swap.legs()[1].interest();
    EXPECT_EQ(floatInterest.size(), floatCoupons.size());
    for (size_t i = 0; i < floatCoupons.size(); i++) { EXPECT_FLOAT_EQ(floatInterest.at(i), floatCoupons.at(i)); }
}