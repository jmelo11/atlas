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
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/parsolver.hpp>
#include <numeric>

using namespace Atlas;

TEST(ParSolver, FixedRateInstrument) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    Indexer<double> indexer;
    indexer.visit(instrument);
    MarketRequest request;
    indexer.setRequest(request);
    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    ParSolver<double> solver(marketData);
    solver.visit(instrument);

    // build QL instrument
    double tmpRate = QuantLib::CashFlows::yield(qlBond.cashflows(), qlBond.NPV(), vars.dayCounter, vars.compounding, vars.frequency, false);
    EXPECT_NEAR(solver.results(), tmpRate, 1e-4);
}

TEST(ParSolver, FixedRateInstrumentDual) {
    // Create a fixed rate instrument
    TestSetup<dual> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    Indexer<dual> indexer;
    indexer.visit(instrument);
    MarketRequest request;
    indexer.setRequest(request);
    SpotMarketDataModel<dual> model(request, vars.store);
    MarketData<dual> marketData = model.marketData();

    ParSolver<dual> solver(marketData);
    solver.visit(instrument);

    double tmpRate = QuantLib::CashFlows::yield(qlBond.cashflows(), qlBond.NPV(), vars.dayCounter, vars.compounding, vars.frequency, false);
    EXPECT_NEAR(val(solver.results()), tmpRate, 1e-4);
}

TEST(ParSolver, FloatingRateInstrument) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    auto& instrument = *vars.atlasFloatBond;
    auto& qlBond     = *vars.qlFloatBond;

    Indexer<double> indexer;
    indexer.visit(instrument);
    MarketRequest request;
    indexer.setRequest(request);
    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    ParSolver<double> solver(marketData);
    solver.visit(instrument);

    // build QL instrument

    double tmpRate = QuantLib::CashFlows::atmRate(qlBond.cashflows(), **vars.discountingTermStructure, false);
    EXPECT_NEAR(solver.results(), 0.0, 1e-6);
}

TEST(ParSolver, FloatingRateInstrumentDual) {
    // Create a fixed rate instrument
    TestSetup<dual> vars;
    auto& instrument = *vars.atlasFloatBond;
    auto& qlBond     = *vars.qlFloatBond;

    Indexer<dual> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    SpotMarketDataModel<dual> model(request, vars.store);

    MarketData<dual> marketData = model.marketData();

    ParSolver<dual> solver(marketData);
    solver.visit(instrument);

    double tmpRate = QuantLib::CashFlows::atmRate(qlBond.cashflows(), **vars.discountingTermStructure, false);
    EXPECT_NEAR(val(solver.results()), 0.0, 1e-6);
}