#ifndef C8B4E847_D420_42A8_A7AC_A1BDBC8381CF
#define C8B4E847_D420_42A8_A7AC_A1BDBC8381CF

#include "testinstrumentcashflowsconstvisitior.hpp"
#include <ql/cashflows/couponpricer.hpp>
#include <ql/cashflows/fixedratecoupon.hpp>
#include <ql/cashflows/floatingratecoupon.hpp>
#include <ql/cashflows/simplecashflow.hpp>
#include <ql/indexes/ibor/usdlibor.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/instruments/bonds/floatingratebond.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>

// Atlas
#include <atlas/fundation/marketstore.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/cashflowaggregation/instrumentcashflowsconstvisitor.hpp>
#include <atlas/visitors/fixingvisitor.hpp>
#include <atlas/visitors/indexingvisitor.hpp>

using namespace Atlas;

TEST(InstrumentCashflowsConstVisitor, FixedRateInstrument) {
    TestInstrumentCashflowsConstVisitor::Common vars;

    InterestRate<double> atlasRate(vars.rate, vars.dayCounter, vars.compounding, vars.frequency);
    FixedRateBulletInstrument<double> atlasInst(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, atlasRate, vars.side);

    Schedule fixSchedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    QuantLib::InterestRate qlRate(vars.rate, vars.dayCounter, vars.compounding, vars.frequency);
    QuantLib::FixedRateBond qlInst(0, vars.notional, fixSchedule, {qlRate.rate()}, vars.dayCounter);

    InstrumentCashflowsConstVisitor cashflowsVisitor;
    cashflowsVisitor(atlasInst);

    for (const auto& cf : qlInst.cashflows()) {
        auto coupon = boost::dynamic_pointer_cast<QuantLib::FixedRateCoupon>(cf);
        if (coupon != nullptr) { EXPECT_NEAR(cashflowsVisitor.getResults().fixedRateCoupons[coupon->date()], coupon->amount(), 1e-6); }

        auto redemption = boost::dynamic_pointer_cast<QuantLib::SimpleCashFlow>(cf);
        if (redemption != nullptr) { EXPECT_NEAR(cashflowsVisitor.getResults().redemptions[redemption->date()], redemption->amount(), 1e-6); }
    };
}

TEST(InstrumentCashflowsConstVisitor, FloatingRateInstrument) {
    TestInstrumentCashflowsConstVisitor::Common vars;
    QuantLib::Settings::instance().evaluationDate() = vars.startDate;
    // QuantLib
    auto qlCurve = boost::make_shared<QuantLib::FlatForward>(vars.startDate, vars.rate, vars.dayCounter, vars.compounding, vars.frequency);
    QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> forecastTermStructure;
    forecastTermStructure.linkTo(qlCurve);

    auto qlIndex = boost::make_shared<QuantLib::IborIndex>("TEST", Period(6, QuantLib::Months), 0, QuantLib::USDCurrency(), QuantLib::NullCalendar(),
                                                           BusinessDayConvention::Unadjusted, false, vars.dayCounter, forecastTermStructure);
    qlIndex->addFixing(vars.startDate, vars.rate);
    Schedule floatSchedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    QuantLib::FloatingRateBond qlInst(0, vars.notional, floatSchedule, qlIndex, vars.dayCounter, QuantLib::Unadjusted, 0);

    // Pricing engine
    QuantLib::Volatility volatility = 0.0;
    QuantLib::Handle<QuantLib::OptionletVolatilityStructure> vol;
    vol = QuantLib::Handle<QuantLib::OptionletVolatilityStructure>(boost::shared_ptr<QuantLib::OptionletVolatilityStructure>(
        new QuantLib::ConstantOptionletVolatility(0, QuantLib::NullCalendar(), QuantLib::Unadjusted, volatility, Actual360())));
    boost::shared_ptr<QuantLib::IborCouponPricer> pricer(new QuantLib::BlackIborCouponPricer);

    boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(forecastTermStructure));

    pricer->setCapletVolatility(vol);
    QuantLib::setCouponPricer(qlInst.cashflows(), pricer);
    qlInst.setPricingEngine(bondEngine);

    // Atlas
    MarketStore<double> store(vars.startDate);
    YieldTermStructure<double> curve = FlatForwardTermStructure<double>(vars.startDate, vars.rate, vars.dayCounter, vars.compounding, vars.frequency);
    InterestRateIndex<double> index(vars.paymentFrequency, RateDefinition(vars.dayCounter, vars.compounding, vars.frequency));
    store.curveManager().addCurveContext("TEST", curve, index);

    FloatingRateBulletInstrument<double> atlasInst(vars.startDate, vars.endDate, vars.notional, vars.spread, index, vars.discountIdx, vars.indexIdx,
                                                   vars.side);
    IndexingVisitor<double> indexingVisitor;
    indexingVisitor(atlasInst);

    auto request = indexingVisitor.getResults();
    SpotMarketDataModel<double> model(request, store);
    MarketData<double> marketData = model.marketData();

    FixingVisitor<double> fixingVisitor(marketData);
    fixingVisitor(atlasInst);

    InstrumentCashflowsConstVisitor cashflowsVisitor;
    cashflowsVisitor(atlasInst);
    auto results = cashflowsVisitor.getResults();
    for (const auto& cf : qlInst.cashflows()) {
        auto coupon = boost::dynamic_pointer_cast<QuantLib::FloatingRateCoupon>(cf);
        if (coupon != nullptr) { EXPECT_NEAR(results.floatingRateCoupons[coupon->date()], coupon->amount(), 1e-6); }

        auto redemption = boost::dynamic_pointer_cast<QuantLib::SimpleCashFlow>(cf);
        if (redemption != nullptr) { EXPECT_NEAR(results.redemptions[redemption->date()], redemption->amount(), 1e-6); }
    };
}

#endif /* C8B4E847_D420_42A8_A7AC_A1BDBC8381CF */
