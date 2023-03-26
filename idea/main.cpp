
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/visitors/cashflowindexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <iostream>

using namespace Atlas;

void pricingFixedRateInstruments() {
    // Create a fixed rate instrument
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    InterestRate rate          = InterestRate(0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    FixedRateBulletInstrument instrument(startDate, endDate, paymentFrequency, notional, rate);

    // Create a curve context store
    CurveContextStore& store_ = CurveContextStore::instance();
    FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    RateIndex index("TEST", Frequency::Annual, Actual360());
    store_.createCurveContext("TEST", curveStrategy, index);

    // set curve context
    instrument.discountCurveContex(store_.at("TEST"));

    CashflowIndexer indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel model(request);

    MarketData marketData = model.simulate(startDate);

    NPVCalculator npvCalculator(marketData);
    npvCalculator.visit(instrument);
    std::cout << "NPV: " << npvCalculator.results() << std::endl;
}

void pricingFloatingRateInstruments() {
    // Create a fixed rate instrument
    Date startDate  = Date(1, Month::Aug, 2020);
    Date endDate    = Date(1, Month::Aug, 2021);
    double notional = 100;
    double spread   = 0.03;

    // Create a curve context store
    CurveContextStore& store_ = CurveContextStore::instance();
    const auto& context       = store_.at("TEST");

    FloatingRateBulletInstrument instrument(startDate, endDate, notional, spread, context);
    // set curve context
    instrument.discountCurveContex(context);

    CashflowIndexer indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel model(request);

    MarketData marketData = model.simulate(startDate);

    NPVCalculator npvCalculator(marketData);
    npvCalculator.visit(instrument);
    std::cout << "NPV: " << npvCalculator.results() << std::endl;
}

int main() {
    Date startDate = Date(1, Month::Aug, 2020);

    pricingFixedRateInstruments();
    pricingFloatingRateInstruments();

    return 0;
}