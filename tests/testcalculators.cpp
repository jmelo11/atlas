#include "commonvars.hpp"
#include "pch.hpp"
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/model/staticcurvemodel.hpp>
#include <atlas/visitors/calculators.hpp>
#include <atlas/visitors/indexers.hpp>

TEST(TestDFIndexer, TestCalculators) {
    Date startDate(1, Month::Jan, 2022);
    Date endDate(1, Month::Jan, 2025);

    DayCounter dayCounter = Thirty360(Thirty360::BondBasis);

    double amount = 100;

    InterestRate rate(0.03, dayCounter, Compounding::Simple, Frequency::Annual);
    Frequency freq = Frequency::Semiannual;
    Atlas::MetaData data;
    Atlas::FixedBullet bullet(startDate, endDate, freq, amount, rate, data);

    bullet.calculateInterests();

    Atlas::DFIndexer indexer;
    indexer.visit(bullet);
    const std::vector<size_t>& idx = bullet.DFIdx();
    // empty indexer
    for (size_t i = 0; i < bullet.interest().size(); i++) { EXPECT_EQ(idx.at(i), i); }
}

TEST(TestFwdIndexer, TestCalculators) {
    Date startDate(1, Month::Jan, 2022);
    Date endDate(1, Month::Jan, 2025);

    DayCounter dayCounter = Thirty360(Thirty360::BondBasis);

    double amount = 100;

    Frequency freq = Frequency::Semiannual;
    Atlas::MetaData data;
    Atlas::FloatingBullet bullet(startDate, endDate, amount, freq, 0, data);

    Atlas::ForwardRatendexer indexer;
    indexer.visit(bullet);

    const std::vector<size_t>& idx = bullet.FWDIdx();

    // empty indexer
    for (size_t i = 0; i < idx.size(); i++) { EXPECT_EQ(idx.at(i), i); }
}

TEST(TestNPVFixedInstrument, TestCalculators) {
    Date startDate(1, Month::Jan, 2022);
    Date endDate(1, Month::Jan, 2025);

    DayCounter dayCounter = Thirty360(Thirty360::BondBasis);

    double amount = 100;

    InterestRate rate(0.03, dayCounter, Compounding::Simple, Frequency::Annual);
    Frequency freq = Frequency::Semiannual;
    Atlas::MetaData data;
    Atlas::FixedBullet bullet(startDate, endDate, freq, amount, rate, data);

    bullet.calculateInterests();

    Atlas::DFIndexer indexer;
    indexer.visit(bullet);

    Atlas::MarketRequest request;
    indexer.setRequest(request);

    std::unique_ptr<Atlas::ZeroRateCurve<Cubic>> discountCurve =
        std::make_unique<Atlas::ZeroRateCurve<Cubic>>(Atlas::buildFlatCurve<Cubic>(startDate, 0.03));
    std::unique_ptr<Atlas::ZeroRateCurve<Cubic>> forecastCurve =
        std::make_unique<Atlas::ZeroRateCurve<Cubic>>(Atlas::buildFlatCurve<Cubic>(startDate, 0.03));

    std::unordered_map<std::string, std::unique_ptr<Atlas::YieldCurve>> discountCurves;
    std::unordered_map<std::string, std::unique_ptr<Atlas::YieldCurve>> forecastCurves;

    discountCurves["undefined"] = std::move(discountCurve);
    forecastCurves["undefined"] = std::move(forecastCurve);

    Atlas::StaticCurveModel model(request, discountCurves, forecastCurves);

    std::vector<Date> evalDates(1, discountCurves["undefined"]->refDate());
    Atlas::Scenario results;
    model.simulate(evalDates, results);

    Atlas::NPVCalculator npvCalc(results[0]);
    bullet.accept(npvCalc);

    double npv = 0;
    for (size_t i = 0; i < bullet.interest().size(); i++)
        npv += (bullet.interest().at(i) + bullet.redemptions().at(i)) * discountCurves["undefined"]->df(bullet.dates().at(i + 1));

    EXPECT_FLOAT_EQ(npv, npvCalc.result());
}

TEST(TestParRateFixedInstrument, TestCalculators) {
    Date startDate(1, Month::Jan, 2022);
    Date endDate(1, Month::Jan, 2025);

    DayCounter dayCounter = Thirty360(Thirty360::BondBasis);

    double amount = 100;

    InterestRate rate(0.03, dayCounter, Compounding::Simple, Frequency::Annual);
    Frequency freq = Frequency::Semiannual;
    Atlas::MetaData data;
    Atlas::FixedBullet bullet(startDate, endDate, freq, amount, rate, data);

    bullet.calculateInterests();

    Atlas::DFIndexer indexer;
    indexer.visit(bullet);

    Atlas::MarketRequest request;
    indexer.setRequest(request);

    std::unique_ptr<Atlas::ZeroRateCurve<Cubic>> discountCurve =
        std::make_unique<Atlas::ZeroRateCurve<Cubic>>(Atlas::buildFlatCurve<Cubic>(startDate, 0.03));
    std::unique_ptr<Atlas::ZeroRateCurve<Cubic>> forecastCurve =
        std::make_unique<Atlas::ZeroRateCurve<Cubic>>(Atlas::buildFlatCurve<Cubic>(startDate, 0.03));

    std::unordered_map<std::string, std::unique_ptr<Atlas::YieldCurve>> discountCurves;
    std::unordered_map<std::string, std::unique_ptr<Atlas::YieldCurve>> forecastCurves;

    discountCurves["undefined"] = std::move(discountCurve);
    forecastCurves["undefined"] = std::move(forecastCurve);

    Atlas::StaticCurveModel model(request, discountCurves, forecastCurves);
    std::vector<Date> evalDates(1, discountCurves["undefined"]->refDate());
    std::vector<Atlas::MarketData> results;
    model.simulate(evalDates, results);

    Atlas::ParRateSolver rateSolver(results[0]);
    bullet.accept(rateSolver);

    EXPECT_FLOAT_EQ(0.02936255, rateSolver.result());
}
