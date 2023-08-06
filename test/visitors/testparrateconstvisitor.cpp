#include "testparrateconstvisitor.hpp"
#include <ql/cashflows/cashflows.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/indexingvisitor.hpp>
#include <atlas/visitors/parrateconstvisitor.hpp>

TEST(ParRateConstVisitor, FixedRateInstrument) {
    // Create a fixed rate instrument
    TestParRateConstVisitor::TestSetup<double> vars;
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
    EXPECT_NEAR(solver.getResults().parRate, tmpRate, 1e-4);
}

TEST(ParRateConstVisitor, FloatingRateInstrument) {
    // Create a fixed rate instrument
    TestParRateConstVisitor::TestSetup<double> vars;
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
    EXPECT_NEAR(solver.getResults().parSpread, 0.0, 1e-6);
}
