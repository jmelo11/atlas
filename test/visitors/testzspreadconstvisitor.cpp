#include "testzspreadconstvisitor.hpp"
#include <ql/cashflows/cashflows.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/indexingvisitor.hpp>
#include <atlas/visitors/zspreadconstvisitor.hpp>

using namespace Atlas;

// New visitors

TEST(ZSpreadConstVisitor, FixedRateInstrument) {
    TestZSpreadConstVisitor::TestSetup<double> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    IndexingVisitor<double> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();

    ZSpreadConstVisitor<double> calculator(marketData, 100.0, vars.dayCounter, vars.compounding, vars.frequency);
    calculator(instrument);

    double zspread = QuantLib::CashFlows::zSpread(qlBond.cashflows(), *vars.discountingTermStructure, 100, vars.dayCounter, vars.compounding,
                                                  vars.frequency, false);
    EXPECT_NEAR(calculator.getResults(), zspread, 1e-6);
}
