#include "../testsetup.hpp"
#include <ql/cashflows/couponpricer.hpp>
#include <ql/indexes/ibor/usdlibor.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/instruments/bonds/floatingratebond.hpp>
#include <ql/instruments/makevanillaswap.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/pricingengines/swap/discountingswapengine.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>

// atlas
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedrateequalredemptioninstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/fixingvisitor.hpp>
#include <atlas/visitors/indexingvisitor.hpp>
#include <atlas/visitors/npvconstvisitor.hpp>
using namespace Atlas;

// New visitors

TEST(NPVConstVisitor, FixedRateInstrument) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    IndexingVisitor<double> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    NPVConstVisitor<double> npvVisitor(marketData);
    npvVisitor(instrument);

    EXPECT_NEAR(npvVisitor.getResults(), qlBond.NPV(), 1e-6);
}

TEST(NPVConstVisitor, FixedRateInstrumentDual) {
    // Create a fixed rate instrument
    TestSetup<dual> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    IndexingVisitor<dual> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<dual> model(request, vars.store);
    MarketData<dual> marketData = model.marketData();

    NPVConstVisitor<dual> npvVisitor(marketData);
    npvVisitor(instrument);

    // for (auto& cashflow : instrument.leg().coupons()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // for (auto& cashflow : instrument.leg().redemptions()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // std::cout << "\n" << std::endl;
    // for (auto& cashflow : qlBond.cashflows()) { std::cout << cashflow->date() << "||" << cashflow->amount() << std::endl; }
    EXPECT_NEAR(val(npvVisitor.getResults()), qlBond.NPV(), 1e-6);
}

TEST(NPVConstVisitor, FloatingRateInstrument) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
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

    EXPECT_NEAR(npvVisitor.getResults(), qlBond.NPV(), 1e-6);
}

TEST(NPVConstVisitor, FloatingRateInstrumentDual) {
    // Create a fixed rate instrument
    TestSetup<dual> vars;
    auto& instrument = *vars.atlasFloatBond;
    auto& qlBond     = *vars.qlFloatBond;

    IndexingVisitor<dual> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<dual> model(request, vars.store);
    MarketData<dual> marketData = model.marketData();

    FixingVisitor fixingVisitor(marketData);
    fixingVisitor(instrument);

    NPVConstVisitor<dual> npvVisitor(marketData);
    npvVisitor(instrument);
    EXPECT_NEAR(val(npvVisitor.getResults()), qlBond.NPV(), 1e-6);
}

TEST(NPVConstVisitor, FxForward) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    double fwdPrice = 825;
    Currency ccy    = CLP();
    double notional = 100'000;

    const auto& curveManager = vars.store.curveManager();
    const auto& fxManager    = vars.store.fxManager();

    size_t idx = curveManager.curveContext("CLP").idx();
    FxForward instrument(vars.startDate, vars.endDate, fwdPrice, CLP(), USD(), notional, Side::Long, idx);

    IndexingVisitor<double> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();

    NPVConstVisitor<double> npvVisitor(marketData);
    npvVisitor(instrument);

    //
    auto& clpCurve = curveManager.curveContext("CLP").curve();
    auto& usdCurve = curveManager.curveContext("USD").curve();
    double spot    = fxManager.exchangeRate(CLP(), USD());
    double proyFx  = spot * usdCurve.discount(vars.endDate) / clpCurve.discount(vars.endDate);
    double df      = clpCurve.discount(vars.endDate);
    double evalNPV = (proyFx - fwdPrice) * df * notional / spot;
    EXPECT_NEAR(npvVisitor.getResults(), evalNPV, 1e-6);

    double notional2 = notional * fwdPrice;

    idx = curveManager.curveContext("USD").idx();
    FxForward instrument2(vars.startDate, vars.endDate, 1 / fwdPrice, USD(), CLP(), notional2, Side::Short, idx);

    IndexingVisitor<double> indexer2;
    indexer2(instrument2);
    MarketRequest request2 = indexer2.getResults();

    SpotMarketDataModel<double> model2(request2, vars.store);
    MarketData<double> marketData2 = model2.marketData();

    NPVConstVisitor<double> npvVisitor2(marketData2);
    npvVisitor2(instrument2);

    EXPECT_NEAR(npvVisitor2.getResults(), evalNPV, 1e-6);
}

TEST(NPVConstVisitor, FixFloatSwap) {
    TestSetup<double> vars;

    const auto& curveManager   = vars.store.curveManager();
    const auto& fxManager      = vars.store.fxManager();
    const auto& index          = curveManager.curveContext("TEST").index();
    size_t idx                 = curveManager.curveContext("TEST").idx();
    Frequency paymentFrequency = Frequency::Quarterly;

    FixFloatSwap<double> swap(vars.startDate, vars.endDate, vars.notional, vars.atlasRate, vars.spread, paymentFrequency, index, Side::Short, idx,
                              idx);

    swap.firstLeg().discountContextIdx(idx);
    swap.secondLeg().discountContextIdx(idx);

    IndexingVisitor<double> indexer;
    indexer(swap);

    MarketRequest request = indexer.getResults();
    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();

    FixingVisitor<double> fixingVisitor(marketData);
    fixingVisitor(swap);
    NPVConstVisitor<double> npvCalculator(marketData);
    npvCalculator(swap);

    Calendar calendar      = QuantLib::NullCalendar();
    Schedule fixSchedule   = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(paymentFrequency).withCalendar(calendar);
    Schedule floatSchedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.indexFrequency).withCalendar(calendar);
    QuantLib::VanillaSwap qlSwap(QuantLib::VanillaSwap::Payer, vars.notional, fixSchedule, vars.qlRate, vars.dayCounter, floatSchedule, vars.qlIndex,
                                 0, vars.curveDayCounter);

    boost::shared_ptr<QuantLib::PricingEngine> engine(new QuantLib::DiscountingSwapEngine(vars.discountingTermStructure));
    qlSwap.setPricingEngine(engine);
    qlSwap.NPV();
    EXPECT_NEAR(npvCalculator.getResults(), qlSwap.NPV(), 1e-6);
}

TEST(NPVConstVisitor, FixedRateInstrumentApplyCcy) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    FixedRateBulletInstrument<double>& instrument = *vars.atlasFixBond;

    instrument.currency(CLP());
    for (auto& coupon : instrument.leg().coupons()) { coupon.applyCcy(true); }
    for (auto& redemption : instrument.leg().redemptions()) { redemption.applyCcy(true); }

    IndexingVisitor<double> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();

    NPVConstVisitor<double> npvVisitor(marketData);
    npvVisitor(instrument);

    // ql
    double npv = 0;
    for (auto& coupon : instrument.leg().coupons()) {
        double fx = marketData.fxs.at(coupon.fxIdx());
        double df = marketData.dfs.at(coupon.dfIdx());
        npv += coupon.amount() * df / fx;
    }

    for (auto& redemption : instrument.leg().redemptions()) {
        double fx = marketData.fxs.at(redemption.fxIdx());
        double df = marketData.dfs.at(redemption.dfIdx());
        npv += redemption.amount() * df / fx;
    }

    EXPECT_NEAR(npvVisitor.getResults(), npv, 1e-6);

    instrument.currency(USD());
    indexer.reset();
    indexer(instrument);

    request = indexer.getResults();
    model.marketRequest(request);
    marketData = model.marketData();

    npvVisitor.reset();
    npvVisitor(instrument);

    npv = 0;
    for (auto& coupon : instrument.leg().coupons()) {
        double fx = marketData.fxs.at(coupon.fxIdx());
        double df = marketData.dfs.at(coupon.dfIdx());
        npv += coupon.amount() * df / fx;
    }

    for (auto& redemption : instrument.leg().redemptions()) {
        double fx = marketData.fxs.at(redemption.fxIdx());
        double df = marketData.dfs.at(redemption.dfIdx());
        npv += redemption.amount() * df / fx;
    }

    EXPECT_NEAR(npvVisitor.getResults(), npv, 1e-6);
}