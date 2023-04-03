#include "../pch.hpp"
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
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/parsolver.hpp>
#include <numeric>

using namespace Atlas;

TEST(ParSolver, FixedRateInstrument) {
    // Create a fixed rate instrument
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Monthly;
    double notional            = 100;
    double rateValue           = 0.03;
    InterestRate<double> rate(rateValue, Thirty360(Thirty360::BondBasis), Compounding::Simple, Frequency::Annual);
    FixedRateBulletInstrument<double> instrument(startDate, endDate, paymentFrequency, notional, rate);

    // Create a curve context store
    CurveContextStore& store_ = CurveContextStore::instance();
    if (!store_.hasContext("TEST")) {
        FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
        RateIndex index("TEST", Frequency::Annual, Actual360());
        store_.createCurveContext("TEST", curveStrategy, index);
    }
    auto& context = store_.at("TEST");
    // set curve context
    instrument.discountCurveContex(context);

    Indexer<double> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel<double> model(request);

    MarketData<double> marketData = model.simulate(startDate);

    ParSolver<double> solver(marketData);
    solver.visit(instrument);

    // build QL instrument
    QuantLib::Settings::instance().evaluationDate() = startDate;

    QuantLib::InterestRate qlRate(rateValue, Thirty360(Thirty360::BondBasis), QuantLib::Compounding::Simple, QuantLib::Frequency::Annual);
    Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(paymentFrequency);
    auto qlBond       = QuantLib::FixedRateBond(0, notional, schedule, {qlRate});

    QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> discountingTermStructure;
    boost::shared_ptr<QuantLib::YieldTermStructure> curve(
        new QuantLib::FlatForward(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual));
    discountingTermStructure.linkTo(curve);
    boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountingTermStructure));
    qlBond.setPricingEngine(bondEngine);
    double tmpRate = QuantLib::CashFlows::atmRate(qlBond.cashflows(), *curve, false);
    EXPECT_NEAR(solver.results(), tmpRate, 1e-4);
}

TEST(ParSolver, FixedRateInstrumentDual) {
    // Create a fixed rate instrument
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Monthly;
    double notional            = 100;
    dual rateValue             = 0.03;
    InterestRate<dual> rate(rateValue, Thirty360(Thirty360::BondBasis), Compounding::Simple, Frequency::Annual);
    FixedRateBulletInstrument<dual> instrument(startDate, endDate, paymentFrequency, notional, rate);

    // Create a curve context store
    CurveContextStore& store_ = CurveContextStore::instance();
    if (!store_.hasContext("TEST")) {
        FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
        RateIndex index("TEST", Frequency::Annual, Actual360());
        store_.createCurveContext("TEST", curveStrategy, index);
    }
    auto& context = store_.at("TEST");
    // set curve context
    instrument.discountCurveContex(context);

    Indexer<dual> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel<dual> model(request);

    MarketData<dual> marketData = model.simulate(startDate);

    ParSolver<dual> solver(marketData);
    solver.visit(instrument);

    // build QL instrument
    QuantLib::Settings::instance().evaluationDate() = startDate;

    QuantLib::InterestRate qlRate(rateValue.val, Thirty360(Thirty360::BondBasis), QuantLib::Compounding::Simple, QuantLib::Frequency::Annual);
    Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(paymentFrequency);
    auto qlBond       = QuantLib::FixedRateBond(0, notional, schedule, {qlRate});

    QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> discountingTermStructure;
    boost::shared_ptr<QuantLib::YieldTermStructure> curve(
        new QuantLib::FlatForward(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual));
    discountingTermStructure.linkTo(curve);
    boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountingTermStructure));
    qlBond.setPricingEngine(bondEngine);
    double tmpRate = QuantLib::CashFlows::atmRate(qlBond.cashflows(), *curve, false);
    EXPECT_NEAR(solver.results().val, tmpRate, 1e-4);
}

TEST(ParSolver, FloatingRateInstrument) {
    // Create a fixed rate instrument
    Date startDate  = Date(1, Month::Aug, 2020);
    Date endDate    = Date(1, Month::Aug, 2021);
    double notional = 100;
    double spread   = 0.01;

    // Create a curve context store
    CurveContextStore& store_ = CurveContextStore::instance();
    if (!store_.hasContext("LIBOR1M")) {
        FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
        RateIndex index("LIBOR1M", Frequency::Monthly, Actual360());
        store_.createCurveContext("LIBOR1M", curveStrategy, index);
    }

    // get context
    auto& context = store_.at("LIBOR1M");
    FloatingRateBulletInstrument<double> instrument(startDate, endDate, notional, spread, context);
    // set curve context
    instrument.discountCurveContex(context);

    Indexer<double> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel<double> model(request);

    MarketData<double> marketData = model.simulate(startDate);

    ParSolver<double> solver(marketData);
    solver.visit(instrument);

    // build QL instrument
    QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> forecastTermStructure;
    QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> discountingTermStructure;

    const boost::shared_ptr<QuantLib::IborIndex> libor1m(new QuantLib::IborIndex("Libor1m", Period(1, QuantLib::Months), 0, QuantLib::USDCurrency(),
                                                                                 QuantLib::NullCalendar(), BusinessDayConvention::Unadjusted, false,
                                                                                 Actual360(), forecastTermStructure));
    libor1m->addFixing(Date(30, Month::July, 2020), 0.03);
    Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(context.index().fixingFrequency());
    auto qlBond       = QuantLib::FloatingRateBond(0, notional, schedule, libor1m, Actual360());

    boost::shared_ptr<QuantLib::YieldTermStructure> curve(
        new QuantLib::FlatForward(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual));
    discountingTermStructure.linkTo(curve);
    forecastTermStructure.linkTo(curve);

    boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountingTermStructure));
    qlBond.setPricingEngine(bondEngine);

    // optionLet volatilities
    QuantLib::Volatility volatility = 0.0;
    QuantLib::Handle<QuantLib::OptionletVolatilityStructure> vol;
    vol = QuantLib::Handle<QuantLib::OptionletVolatilityStructure>(boost::shared_ptr<QuantLib::OptionletVolatilityStructure>(
        new QuantLib::ConstantOptionletVolatility(0, QuantLib::NullCalendar(), QuantLib::Unadjusted, volatility, Actual360())));
    boost::shared_ptr<QuantLib::IborCouponPricer> pricer(new QuantLib::BlackIborCouponPricer);

    pricer->setCapletVolatility(vol);
    QuantLib::setCouponPricer(qlBond.cashflows(), pricer);
    double tmpRate = QuantLib::CashFlows::atmRate(qlBond.cashflows(), *curve, false);
    EXPECT_NEAR(solver.results(), 0.0, 1e-6);
}

TEST(ParSolver, FloatingRateInstrumentDual) {
    // Create a fixed rate instrument
    Date startDate  = Date(1, Month::Aug, 2020);
    Date endDate    = Date(1, Month::Aug, 2021);
    double notional = 100;
    dual spread   = 0.01;

    // Create a curve context store
    CurveContextStore& store_ = CurveContextStore::instance();
    if (!store_.hasContext("LIBOR1M")) {
        FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
        RateIndex index("LIBOR1M", Frequency::Monthly, Actual360());
        store_.createCurveContext("LIBOR1M", curveStrategy, index);
    }

    // get context
    auto& context = store_.at("LIBOR1M");
    FloatingRateBulletInstrument<dual> instrument(startDate, endDate, notional, spread, context);
    // set curve context
    instrument.discountCurveContex(context);

    Indexer<dual> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel<dual> model(request);

    MarketData<dual> marketData = model.simulate(startDate);

    ParSolver<dual> solver(marketData);
    solver.visit(instrument);

    // build QL instrument
    QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> forecastTermStructure;
    QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> discountingTermStructure;

    const boost::shared_ptr<QuantLib::IborIndex> libor1m(new QuantLib::IborIndex("Libor1m", Period(1, QuantLib::Months), 0, QuantLib::USDCurrency(),
                                                                                 QuantLib::NullCalendar(), BusinessDayConvention::Unadjusted, false,
                                                                                 Actual360(), forecastTermStructure));
    libor1m->addFixing(Date(30, Month::July, 2020), 0.03);
    Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(context.index().fixingFrequency());
    auto qlBond       = QuantLib::FloatingRateBond(0, notional, schedule, libor1m, Actual360());

    boost::shared_ptr<QuantLib::YieldTermStructure> curve(
        new QuantLib::FlatForward(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual));
    discountingTermStructure.linkTo(curve);
    forecastTermStructure.linkTo(curve);

    boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountingTermStructure));
    qlBond.setPricingEngine(bondEngine);

    // optionLet volatilities
    QuantLib::Volatility volatility = 0.0;
    QuantLib::Handle<QuantLib::OptionletVolatilityStructure> vol;
    vol = QuantLib::Handle<QuantLib::OptionletVolatilityStructure>(boost::shared_ptr<QuantLib::OptionletVolatilityStructure>(
        new QuantLib::ConstantOptionletVolatility(0, QuantLib::NullCalendar(), QuantLib::Unadjusted, volatility, Actual360())));
    boost::shared_ptr<QuantLib::IborCouponPricer> pricer(new QuantLib::BlackIborCouponPricer);

    pricer->setCapletVolatility(vol);
    QuantLib::setCouponPricer(qlBond.cashflows(), pricer);
    double tmpRate = QuantLib::CashFlows::atmRate(qlBond.cashflows(), *curve, false);
    EXPECT_NEAR(solver.results().val, 0.0, 1e-6);
}