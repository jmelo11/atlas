#include "../testsetup.hpp"
#include <ql/cashflows/cashflows.hpp>
#include <ql/cashflows/couponpricer.hpp>
#include <ql/indexes/ibor/usdlibor.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/instruments/bonds/floatingratebond.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/visitors/indexingvisitor.hpp>
#include <atlas/visitors/zspreadconstvisitor.hpp>

using namespace Atlas;

// New visitors

TEST(ZSpreadConstVisitor, FixedRateInstrument) {
    TestSetup<double> vars;
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

TEST(ZSpreadConstVisitor, FixedRateInstrumentDual) {
    TestSetup<dual> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    IndexingVisitor<dual> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<dual> model(request, vars.store);
    MarketData<dual> marketData = model.marketData();

    ZSpreadConstVisitor<dual> calculator(marketData, 100.0, vars.dayCounter, vars.compounding, vars.frequency, 0.0001, 1e-6, 1000);
    calculator(instrument);

    double zspread = QuantLib::CashFlows::zSpread(qlBond.cashflows(), *vars.discountingTermStructure, 100, vars.dayCounter, vars.compounding,
                                                  vars.frequency, false);
    EXPECT_NEAR(val(calculator.getResults()), zspread, 1e-6);
}