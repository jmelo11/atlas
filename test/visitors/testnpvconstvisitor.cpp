#include "testnpvconstvisitor.hpp"
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/fixingvisitor.hpp>
#include <atlas/visitors/indexingvisitor.hpp>
#include <atlas/visitors/npvconstvisitor.hpp>

TEST(NPVConstVisitor, FixedRateInstrument) {
    // Create a fixed rate instrument
    TestNPVConstVisitor::TestSetup<double> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    IndexingVisitor<double> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    NPVConstVisitor<double> npvVisitor(marketData);
    npvVisitor(instrument);

    double pv = npvVisitor.getResults().redemptionsNPV + npvVisitor.getResults().fixedRateCouponsNPV;
    EXPECT_NEAR(pv, qlBond.NPV(), 1e-6);
}

TEST(NPVConstVisitor, FloatingRateInstrument) {
    TestNPVConstVisitor::TestSetup<double> vars;
    auto& instrument = *vars.atlasFloatBond;
    auto& qlBond     = *vars.qlFloatBond;

    IndexingVisitor<double> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();

    FixingVisitor fixingVisitor(marketData);
    fixingVisitor(instrument);

    NPVConstVisitor<double> npvVisitor(marketData);
    npvVisitor(instrument);

    double pv = npvVisitor.getResults().redemptionsNPV + npvVisitor.getResults().floatingRateCouponsNPV;
    EXPECT_NEAR(pv, qlBond.NPV(), 1e-6);
}
