#include "../pch.hpp"
#include "general.hpp"
#include <ql/cashflows/cashflows.hpp>
#include <ql/cashflows/couponpricer.hpp>
#include <ql/indexes/ibor/usdlibor.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/instruments/bonds/floatingratebond.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/zspreadcalculator.hpp>

using namespace Atlas;

TEST(ZSpreadSolver, FixedRateInstrument) {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Monthly;
    double notional            = 100;
    double rateValue           = 0.05;
    InterestRate<double> rate(rateValue, Thirty360(Thirty360::BondBasis), Compounding::Simple, Frequency::Annual);
    QuantLib::InterestRate qlRate(rateValue, Thirty360(Thirty360::BondBasis), QuantLib::Compounding::Simple, QuantLib::Frequency::Annual);

    FixedRateBulletInstrument<double> instrument(startDate, endDate, paymentFrequency, notional, rate);

    // Create a curve context store
    MarketStore<double> store_;
    FlatForwardStrategy<double> curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    YieldTermStructure<double> curve_(std::make_unique<FlatForwardStrategy<double>>(curveStrategy));
    RateIndex index("TEST", Frequency::Annual, Actual360());
    store_.createCurveContext("TEST", curve_, index);
    auto& context = store_.curveContext("TEST");
    // set curve context
    instrument.discountCurveContex(context);

    Indexer<double> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel<double> model(request, store_);
    MarketData<double> marketData = model.simulate(startDate);
    ZSpreadCalculator<double> calculator(marketData, 100.0);
    calculator.visit(instrument);

    QuantLib::Settings::instance().evaluationDate() = startDate;

    Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(paymentFrequency);
    auto qlBond       = QuantLib::FixedRateBond(0, notional, schedule, {qlRate});

    // for (auto& cashflow : instrument.leg().coupons()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // for (auto& cashflow : instrument.leg().redemptions()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // std::cout << "\n" << std::endl;
    // for (auto& cashflow : qlBond.cashflows()) { std::cout << cashflow->date() << "||" << cashflow->amount() << std::endl; }

    QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> discountingTermStructure;
    boost::shared_ptr<QuantLib::YieldTermStructure> curve(
        new QuantLib::FlatForward(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual));
    discountingTermStructure.linkTo(curve);
    boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountingTermStructure));
    qlBond.setPricingEngine(bondEngine);

    double zspread = QuantLib::CashFlows::zSpread(qlBond.cashflows(), *discountingTermStructure, 100, Actual360(), Compounding::Compounded,
                                                  Frequency::Semiannual, false);
    EXPECT_NEAR(calculator.results(), zspread, 1e-6);
}

TEST(ZSpreadSolver, FixedRateInstrumentDual) {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Monthly;
    double notional            = 100;
    dual rateValue             = 0.05;
    InterestRate<dual> rate(rateValue, Thirty360(Thirty360::BondBasis), Compounding::Simple, Frequency::Annual);
    QuantLib::InterestRate qlRate(val(rateValue), Thirty360(Thirty360::BondBasis), QuantLib::Compounding::Simple, QuantLib::Frequency::Annual);

    FixedRateBulletInstrument<dual> instrument(startDate, endDate, paymentFrequency, notional, rate);

    // Create a curve context store
     MarketStore<dual> store_;
    FlatForwardStrategy<dual> curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    YieldTermStructure<dual> curve_(std::make_unique<FlatForwardStrategy<dual>>(curveStrategy));
    RateIndex index("TEST", Frequency::Annual, Actual360());
    store_.createCurveContext("TEST", curve_, index);

    auto& context = store_.curveContext("TEST");
    // set curve context
    instrument.discountCurveContex(context);

    Indexer<dual> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel<dual> model(request, store_);
    MarketData<dual> marketData = model.simulate(startDate);
    ZSpreadCalculator<dual> calculator(marketData, 100.0);
    calculator.visit(instrument);

    QuantLib::Settings::instance().evaluationDate() = startDate;

    Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(paymentFrequency);
    auto qlBond       = QuantLib::FixedRateBond(0, notional, schedule, {qlRate});

    // for (auto& cashflow : instrument.leg().coupons()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // for (auto& cashflow : instrument.leg().redemptions()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // std::cout << "\n" << std::endl;
    // for (auto& cashflow : qlBond.cashflows()) { std::cout << cashflow->date() << "||" << cashflow->amount() << std::endl; }

    QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> discountingTermStructure;
    boost::shared_ptr<QuantLib::YieldTermStructure> curve(
        new QuantLib::FlatForward(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual));
    discountingTermStructure.linkTo(curve);
    boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountingTermStructure));
    qlBond.setPricingEngine(bondEngine);

    double zspread = QuantLib::CashFlows::zSpread(qlBond.cashflows(), *discountingTermStructure, 100, Actual360(), Compounding::Compounded,
                                                  Frequency::Semiannual, false);
    EXPECT_NEAR(val(calculator.results()), zspread, 1e-6);
}