#include "../pch.hpp"
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/visitors/cashflowindexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>

using namespace Atlas;

TEST(NPVCalculator, FixedRateInstrument) {
    // Create a fixed rate instrument
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    InterestRate rate          = InterestRate(0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    FixedRateBulletInstrument instrument(startDate, endDate, paymentFrequency, notional, rate);

    // Create a curve context store
    CurveContextStore store_;
    auto curve = std::make_unique<QuantLib::FlatForward>(startDate, 0.03, Actual360());
    auto index = std::make_unique<RateIndex>("TEST", Frequency::Annual, Actual360());
    store_.createCurveContext("TEST", std::move(curve), std::move(index));

    // set curve context
    instrument.discountCurveContex(store_.at("TEST"));

    CashflowIndexer indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel model(request, store_);

    std::vector<Date> evalDates = {startDate};
    MarketData marketData       = model.simulate();

    NPVCalculator npvCalculator(marketData);
    npvCalculator.visit(instrument);
    EXPECT_NEAR(npvCalculator.results(), 100.0, 1e-6);
}

TEST(NPVCalculator, FloatingRateInstrument) {
    // Create a fixed rate instrument
    Date startDate  = Date(1, Month::Aug, 2020);
    Date endDate    = Date(1, Month::Aug, 2021);
    double notional = 100;
    double spread   = 0.03;

    // Create a curve context store
    CurveContextStore store_;
    auto curve = std::make_unique<QuantLib::FlatForward>(startDate, 0.03, Actual360());
    auto index = std::make_unique<RateIndex>("TEST", Frequency::Annual, Actual360());
    store_.createCurveContext("TEST", std::move(curve), std::move(index));

    // get context
    auto context = store_.at("TEST");
    FloatingRateBulletInstrument instrument(startDate, endDate, notional, spread, context);
    // set curve context
    instrument.discountCurveContex(context);

    CashflowIndexer indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel model(request, store_);

    std::vector<Date> evalDates = {startDate};
    MarketData marketData       = model.simulate();

    NPVCalculator npvCalculator(marketData);
    npvCalculator.visit(instrument);
    EXPECT_NEAR(npvCalculator.results(), 100.0, 1e-6);
}
