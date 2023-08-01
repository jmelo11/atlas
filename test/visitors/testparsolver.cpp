#include "../testsetup.hpp"
#include <ql/cashflows/cashflows.hpp>
#include <ql/cashflows/couponpricer.hpp>
#include <ql/indexes/ibor/usdlibor.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/instruments/bonds/floatingratebond.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/visitors/indexingvisitor.hpp>
#include <atlas/visitors/parrateconstvisitor.hpp>
#include <numeric>
using namespace Atlas;

TEST(ParRateConstVisitor, FixedRateInstrument) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    IndexingVisitor<double> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();

    ParRateConstVisitor<double> solver(marketData);
    solver(instrument);

    // build QL instrument
    double tmpRate = QuantLib::CashFlows::yield(qlBond.cashflows(), qlBond.NPV(), vars.dayCounter, vars.compounding, vars.frequency, false);
    EXPECT_NEAR(solver.getResults()["rate"], tmpRate, 1e-4);
}

TEST(ParRateConstVisitor, FixedRateInstrumentDual) {
    // Create a fixed rate instrument
    TestSetup<dual> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    IndexingVisitor<dual> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<dual> model(request, vars.store);
    MarketData<dual> marketData = model.marketData();

    ParRateConstVisitor<dual> solver(marketData);
    solver(instrument);

    double tmpRate = QuantLib::CashFlows::yield(qlBond.cashflows(), qlBond.NPV(), vars.dayCounter, vars.compounding, vars.frequency, false);
    EXPECT_NEAR(val(solver.getResults()["rate"]), tmpRate, 1e-4);
}

TEST(ParRateConstVisitor, FloatingRateInstrument) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    auto& instrument = *vars.atlasFloatBond;
    auto& qlBond     = *vars.qlFloatBond;

    IndexingVisitor<double> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();

    ParRateConstVisitor<double> solver(marketData);
    solver(instrument);

    // build QL instrument

    double tmpRate = QuantLib::CashFlows::atmRate(qlBond.cashflows(), **vars.discountingTermStructure, false);
    EXPECT_NEAR(solver.getResults()["spread"], 0.0, 1e-6);
}

TEST(ParRateConstVisitor, FloatingRateInstrumentDual) {
    TestSetup<dual> vars;
    auto& instrument = *vars.atlasFloatBond;
    auto& qlBond     = *vars.qlFloatBond;

    IndexingVisitor<dual> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<dual> model(request, vars.store);

    MarketData<dual> marketData = model.marketData();

    ParRateConstVisitor<dual> solver(marketData);
    solver(instrument);

    double tmpRate = QuantLib::CashFlows::atmRate(qlBond.cashflows(), **vars.discountingTermStructure, false);
    EXPECT_NEAR(val(solver.getResults()["spread"]), 0.0, 1e-6);
}
