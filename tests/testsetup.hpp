#ifndef ED4FE09D_1DB6_49CB_AD5A_B4C22DD2A839
#define ED4FE09D_1DB6_49CB_AD5A_B4C22DD2A839

#include "pch.hpp"
#include <ql/cashflows/couponpricer.hpp>
#include <ql/indexes/ibor/usdlibor.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/instruments/bonds/floatingratebond.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/fundation/marketstore.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/rates/rateindex.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>

using namespace Atlas;

template <typename adouble>
struct TestSetup {
    TestSetup() {
        QuantLib::Settings::instance().evaluationDate() = evalDate;
        createCurves();
        createQuantLibIndex();
        createInstrumentRates();
        createInstruments();
    }

    // session parameters
    Date evalDate = Date(1, Month::Aug, 2020);

    // instrument parameters
    Date startDate  = evalDate;
    Date endDate    = Date(1, Month::Aug, 2021);
    double notional = 100;
    adouble spread  = 0.0;
    adouble rate    = 0.03;

    DayCounter dayCounter      = Actual360();
    Frequency frequency        = Frequency::Annual;
    Compounding compounding    = Compounding::Simple;
    Frequency paymentFrequency = Frequency::Semiannual;

    QuantLib::InterestRate qlRate;
    InterestRate<adouble> atlasRate;

    // curve parameters
    Date curveRefDate            = evalDate;
    adouble curveRate            = 0.05;
    adouble usdCurveRate         = 0.03;
    DayCounter curveDayCounter   = Actual360();
    Compounding curveCompounding = Compounding::Simple;
    Frequency curveFrequency     = Frequency::Annual;
    Frequency indexFrequency     = Frequency::Quarterly;

    // atlas curves & indexes
    MarketStore<adouble> store = MarketStore<adouble>(evalDate);

    // ql curves & indexes
    QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> forecastTermStructure;
    QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> discountingTermStructure;
    boost::shared_ptr<QuantLib::IborIndex> qlIndex;

    // atlas instruments
    FixedRateBulletInstrument<adouble>* atlasFixBond;
    FloatingRateBulletInstrument<adouble>* atlasFloatBond;

    // ql instruments
    QuantLib::FixedRateBond* qlFixBond;
    QuantLib::FloatingRateBond* qlFloatBond;

    // methods
    void createCurves() {
        // create atlas curves
        FlatForwardStrategy<adouble> curveStrategy(curveRefDate, curveRate, curveDayCounter, curveCompounding, curveFrequency);
        YieldTermStructure<adouble> curve(std::make_unique<FlatForwardStrategy<adouble>>(curveStrategy));
        RateIndex<adouble> index(curveRefDate, indexFrequency, curveDayCounter);
        store.addCurve("TEST", curve, index);

        FlatForwardStrategy<adouble> usdCurveStrategy(curveRefDate, usdCurveRate, curveDayCounter, curveCompounding, curveFrequency);
        YieldTermStructure<adouble> usdCurve(std::make_unique<FlatForwardStrategy<adouble>>(usdCurveStrategy));
        store.addCurve("CLP", curve, index);
        store.addCurve("USD", usdCurve, index);

        // for fx testing
        adouble exchange = 800;
        store.addExchangeRate(CLP(), USD(), exchange);
        store.riskFreeCurveIdx(CLP(), "CLP");
        store.riskFreeCurveIdx(USD(), "USD");

        // create ql curves
        boost::shared_ptr<QuantLib::YieldTermStructure> qlCurve;
        if constexpr (std::is_same_v<adouble, double>) {
            qlCurve = boost::make_shared<QuantLib::FlatForward>(curveRefDate, curveRate, curveDayCounter, curveCompounding, curveFrequency);
        } else {
            qlCurve = boost::make_shared<QuantLib::FlatForward>(curveRefDate, val(curveRate), curveDayCounter, curveCompounding, curveFrequency);
        }
        discountingTermStructure.linkTo(qlCurve);
        forecastTermStructure.linkTo(qlCurve);
    }

    void createQuantLibIndex() {
        qlIndex = boost::make_shared<QuantLib::IborIndex>("TEST", Period(3, QuantLib::Months), 0, QuantLib::USDCurrency(), QuantLib::NullCalendar(),
                                                          BusinessDayConvention::Unadjusted, false, curveDayCounter, forecastTermStructure);
        qlIndex->addFixing(Date(30, Month::July, 2020), 0.03);
    }

    void createInstrumentRates() {
        if constexpr (std::is_same_v<adouble, double>) {
            qlRate = QuantLib::InterestRate(rate, dayCounter, compounding, frequency);
        } else {
            qlRate = QuantLib::InterestRate(val(rate), dayCounter, compounding, frequency);
        }
        atlasRate = InterestRate<adouble>(rate, dayCounter, compounding, frequency);
    }

    void createInstruments() {
        // create ql instruments
        Schedule fixSchedule = MakeSchedule().from(startDate).to(endDate).withFrequency(paymentFrequency);
        qlFixBond            = new QuantLib::FixedRateBond(0, notional, fixSchedule, {qlRate});

        boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountingTermStructure));
        qlFixBond->setPricingEngine(bondEngine);

        Schedule floatSchedule = MakeSchedule().from(startDate).to(endDate).withFrequency(indexFrequency);

        qlFloatBond = new QuantLib::FloatingRateBond(0, notional, floatSchedule, qlIndex, curveDayCounter, QuantLib::Unadjusted, 0);

        QuantLib::Volatility volatility = 0.0;
        QuantLib::Handle<QuantLib::OptionletVolatilityStructure> vol;
        vol = QuantLib::Handle<QuantLib::OptionletVolatilityStructure>(boost::shared_ptr<QuantLib::OptionletVolatilityStructure>(
            new QuantLib::ConstantOptionletVolatility(0, QuantLib::NullCalendar(), QuantLib::Unadjusted, volatility, Actual360())));
        boost::shared_ptr<QuantLib::IborCouponPricer> pricer(new QuantLib::BlackIborCouponPricer);

        pricer->setCapletVolatility(vol);
        QuantLib::setCouponPricer(qlFloatBond->cashflows(), pricer);
        qlFloatBond->setPricingEngine(bondEngine);
        // create atlas instruments
        atlasFixBond = new FixedRateBulletInstrument<adouble>(startDate, endDate, paymentFrequency, notional, atlasRate, store.curveContext("TEST"));
        atlasFloatBond = new FloatingRateBulletInstrument<adouble>(startDate, endDate, notional, spread, store.rateIndexContext("TEST"),
                                                                   store.curveContext("TEST"));
    }
};

#endif /* ED4FE09D_1DB6_49CB_AD5A_B4C22DD2A839 */
