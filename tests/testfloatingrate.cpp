#include "commonvars.hpp"
#include "pch.hpp"
#include <atlas/curves/curve.hpp>
#include <atlas/instruments/fixfloatswap.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/model/staticcurvemodel.hpp>
#include <atlas/visitors/calculators.hpp>
#include <atlas/visitors/indexers.hpp>

std::vector<double> calculateCoupons(Atlas::FloatingRateInstrument& inst, const std::unique_ptr<Atlas::YieldCurve>& curve) {
    std::vector<double> result;
    auto dates = inst.dates();
    for (size_t i = 1; i < dates.size(); i++) {
        double yf = commonVars.dayCounter.yearFraction(dates[i - 1], dates[i]);
        if (yf > 0.0 + QL_EPSILON) {
            double r = curve->forwardRate(dates[i - 1], dates[i]) + inst.spread();
            InterestRate rate(r, commonVars.dayCounter, Compounding::Simple, Frequency::Annual);
            double amount = inst.outstanding()[i - 1];
            result.push_back(amount * (rate.compoundFactor(dates[i - 1], dates[i]) - 1));
        }
    }
    return result;
}

TEST(TestFloatingBulletConsistency, TestFloatingRate) {
    Atlas::FloatingBullet bullet(commonVars.startDate, commonVars.endDate, commonVars.amount, commonVars.paymentFreq, commonVars.spread,
                                 commonVars.metaData);
    bullet.calculateOutstanding();
    std::unique_ptr<Atlas::ZeroRateCurve<Cubic>> forecastCurve =
        std::make_unique<Atlas::ZeroRateCurve<Cubic>>(Atlas::buildFlatCurve<Cubic>(commonVars.startDate, commonVars.curveRate));

    std::unordered_map<std::string, std::unique_ptr<Atlas::YieldCurve>> discountCurves;
    std::unordered_map<std::string, std::unique_ptr<Atlas::YieldCurve>> forecastCurves;

    forecastCurves["undefined"] = std::move(forecastCurve);

    Atlas::ForwardRatendexer indexer;
    indexer.visit(bullet);
    Atlas::MarketRequest request;
    indexer.setRequest(request);

    Atlas::StaticCurveModel model(request, discountCurves, forecastCurves);
    std::vector<Date> evalDates(1, Date(1, Month::August, 2022));
    std::vector<Atlas::MarketData> results;
    model.simulate(evalDates, results);

    Atlas::InterestCalculator caculator(results[0]);
    caculator.visit(bullet);

    std::vector<double> coupons = calculateCoupons(bullet, forecastCurves["undefined"]);

    EXPECT_EQ(bullet.interest().size(), coupons.size());
    for (size_t i = 0; i < coupons.size(); i++) { EXPECT_FLOAT_EQ(bullet.interest().at(i), coupons.at(i)); }
}

TEST(TestFloatingCustomConsistency, TestFloatingRate) {
    // define bond
    double notional         = 100;
    std::vector<Date> dates = commonVars.schedule.dates();
    std::vector<double> redemptions(commonVars.schedule.size() - 1, notional / (commonVars.schedule.size() - 1));

    Atlas::FloatingCustomStructure bond(dates, redemptions, commonVars.rate, commonVars.metaData);
    bond.calculateOutstanding();

    // market curves for pricing
    std::unique_ptr<Atlas::ZeroRateCurve<Cubic>> forecastCurve =
        std::make_unique<Atlas::ZeroRateCurve<Cubic>>(Atlas::buildFlatCurve<Cubic>(commonVars.startDate, commonVars.curveRate));

    std::unordered_map<std::string, std::unique_ptr<Atlas::YieldCurve>> discountCurves;
    std::unordered_map<std::string, std::unique_ptr<Atlas::YieldCurve>> forecastCurves;

    forecastCurves["undefined"] = std::move(forecastCurve);

    // index rates
    Atlas::ForwardRatendexer indexer;
    indexer.visit(bond);

    // make request
    Atlas::MarketRequest request;
    indexer.setRequest(request);

    // simulate
    Atlas::StaticCurveModel model(request, discountCurves, forecastCurves);
    std::vector<Date> evalDates = std::vector<Date>(1, commonVars.evalDate);
    std::vector<Atlas::MarketData> results;
    model.simulate(evalDates, results);

    // set simulated rates
    Atlas::InterestCalculator caculator(results[0]);
    caculator.visit(bond);

    std::vector<double> coupons = calculateCoupons(bond, forecastCurves["undefined"]);

    // check number of coupons
    EXPECT_EQ(bond.interest().size(), coupons.size());

    // check interest
    for (size_t i = 0; i < coupons.size(); i++) { EXPECT_FLOAT_EQ(bond.interest().at(i), coupons.at(i)); }
}