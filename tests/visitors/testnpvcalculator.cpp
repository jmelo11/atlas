#include "../testsetup.hpp"
#include <ql/cashflows/couponpricer.hpp>
#include <ql/indexes/ibor/usdlibor.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/instruments/bonds/floatingratebond.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>

using namespace Atlas;

TEST(NPVCalculator, FixedRateInstrument) {
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
    NPVCalculator<double> npvCalculator(marketData);
    npvCalculator.visit(instrument);

    EXPECT_NEAR(npvCalculator.results(), qlBond.NPV(), 1e-6);
}

TEST(NPVCalculator, FixedRateInstrumentDual) {
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
    NPVCalculator<dual> npvCalculator(marketData);
    npvCalculator.visit(instrument);

    // for (auto& cashflow : instrument.leg().coupons()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // for (auto& cashflow : instrument.leg().redemptions()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // std::cout << "\n" << std::endl;
    // for (auto& cashflow : qlBond.cashflows()) { std::cout << cashflow->date() << "||" << cashflow->amount() << std::endl; }
    EXPECT_NEAR(val(npvCalculator.results()), qlBond.NPV(), 1e-6);
}

TEST(NPVCalculator, FloatingRateInstrument) {
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
    NPVCalculator<double> npvCalculator(marketData);
    npvCalculator.visit(instrument);

    EXPECT_NEAR(npvCalculator.results(), qlBond.NPV(), 1e-6);
}

TEST(NPVCalculator, FloatingRateInstrumentDual) {
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
    NPVCalculator<dual> npvCalculator(marketData);
    npvCalculator.visit(instrument);

    EXPECT_NEAR(val(npvCalculator.results()), qlBond.NPV(), 1e-6);
}

TEST(NPVCalculator, FxForward) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    double fwdPrice = 825;
    Currency ccy = CLP();
    double notional = 100'000;
    FxForward instrument(vars.startDate, vars.endDate, fwdPrice, CLP(), USD(), notional, FxForward<double>::BUY, vars.store.curveContext("CLP"));

    Indexer<double> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);
    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    NPVCalculator<double> npvCalculator(marketData);
    npvCalculator.visit(instrument);

    // 
    auto& clpCurve = vars.store.curveContext("CLP").object();
    auto& usdCurve = vars.store.curveContext("USD").object();
    double spot = vars.store.exchange(CLP(),USD());
    double proyFx = spot * usdCurve.discount(vars.endDate) / clpCurve.discount(vars.endDate);
    double df = clpCurve.discount(vars.endDate);
    double evalNPV = (proyFx-fwdPrice)*df*notional/spot;
    EXPECT_NEAR(npvCalculator.results(), evalNPV, 1e-6);

    double notional2 = notional*fwdPrice;
    FxForward instrument2(vars.startDate, vars.endDate, 1/fwdPrice, USD(), CLP(), notional2, FxForward<double>::SELL, vars.store.curveContext("USD"));
    Indexer<double> indexer2;
    indexer2.visit(instrument2);
    MarketRequest request2;
    indexer2.setRequest(request2);
    SpotMarketDataModel<double> model2(request2, vars.store);
    MarketData<double> marketData2 = model2.marketData();
    NPVCalculator<double> npvCalculator2(marketData2);
    npvCalculator2.visit(instrument2);

    EXPECT_NEAR(npvCalculator2.results(), evalNPV, 1e-6);
}
