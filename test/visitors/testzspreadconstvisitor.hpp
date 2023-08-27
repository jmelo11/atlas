#ifndef B8AC5D97_78E1_4A3A_B0E8_C81B7761D235
#define B8AC5D97_78E1_4A3A_B0E8_C81B7761D235

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
#include <atlas/others/interpolations/linearinterpolation.hpp>
#include <atlas/rates/index/iborindex.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/rates/yieldtermstructure/zeroratecurve.hpp>
#include <gtest/gtest.h>

using namespace Atlas;
namespace TestZSpreadConstVisitor {

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
        adouble clpCurveRate         = 0.07;
        DayCounter curveDayCounter   = Actual360();
        Compounding curveCompounding = Compounding::Simple;
        Frequency curveFrequency     = Frequency::Annual;
        Frequency indexFrequency     = Frequency::Quarterly;

        // atlas curves & indexes
        MarketStore<adouble> store = MarketStore<adouble>(evalDate);

        // ql curves & indexes
        QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> forecastTermStructure;
        QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> discountingTermStructure;
        QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> clpDiscountingTermStructure;

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

            YieldTermStructure<adouble> curve =
                FlatForwardTermStructure<adouble>(curveRefDate, curveRate, curveDayCounter, curveCompounding, curveFrequency);
            Index<adouble> index = IborIndex(curve, indexFrequency, RateDefinition(curveDayCounter, curveCompounding, Frequency::Annual));
            store.curveManager().addCurveContext("TEST", curve, index);

            YieldTermStructure<adouble> clpCurve =
                FlatForwardTermStructure<adouble>(curveRefDate, clpCurveRate, curveDayCounter, curveCompounding, curveFrequency);
            store.curveManager().addCurveContext("CLP", clpCurve, index, CLP(), true);

            YieldTermStructure<adouble> usdCurve =
                FlatForwardTermStructure<adouble>(curveRefDate, usdCurveRate, curveDayCounter, curveCompounding, curveFrequency);

            store.curveManager().addCurveContext("USD", usdCurve, index, USD(), true);

            std::vector<adouble> rates = {0.01, 0.02, 0.03, 0.04, 0.05};
            std::vector<Date> dates    = {Date(1, Month::Aug, 2020), Date(1, Month::Aug, 2021), Date(1, Month::Aug, 2023), Date(1, Month::Aug, 2025),
                                          Date(1, Month::Aug, 2030)};

            YieldTermStructure<adouble> zeroCurve = ZeroRateTermStructure<LinearInterpolator, adouble>(dates, rates, curveDayCounter);
            store.curveManager().addCurveContext("Zero", zeroCurve, index);

            // for fx testing
            adouble exchange = 800;
            store.fxManager().addExchangeRate(CLP(), USD(), exchange);

            // create ql curves
            boost::shared_ptr<QuantLib::YieldTermStructure> qlCurve;
            boost::shared_ptr<QuantLib::YieldTermStructure> qlClpCurve;
            if constexpr (std::is_same_v<adouble, double>) {
                qlCurve    = boost::make_shared<QuantLib::FlatForward>(curveRefDate, curveRate, curveDayCounter, curveCompounding, curveFrequency);
                qlClpCurve = boost::make_shared<QuantLib::FlatForward>(curveRefDate, clpCurveRate, curveDayCounter, curveCompounding, curveFrequency);
            } else {
                qlCurve = boost::make_shared<QuantLib::FlatForward>(curveRefDate, val(curveRate), curveDayCounter, curveCompounding, curveFrequency);
                qlClpCurve =
                    boost::make_shared<QuantLib::FlatForward>(curveRefDate, val(clpCurveRate), curveDayCounter, curveCompounding, curveFrequency);
            }
            discountingTermStructure.linkTo(qlCurve);
            forecastTermStructure.linkTo(qlCurve);
            clpDiscountingTermStructure.linkTo(qlClpCurve);
        }

        void createQuantLibIndex() {
            qlIndex =
                boost::make_shared<QuantLib::IborIndex>("TEST", Period(3, QuantLib::Months), 0, QuantLib::USDCurrency(), QuantLib::NullCalendar(),
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
            qlFixBond            = new QuantLib::FixedRateBond(0, notional, fixSchedule, {qlRate.rate()}, dayCounter);

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

            const auto& curveManager = store.curveManager();
            atlasFixBond             = new FixedRateBulletInstrument<adouble>(startDate, endDate, paymentFrequency, notional, atlasRate,
                                                                  curveManager.curveContext("TEST").idx());
            atlasFloatBond =
                new FloatingRateBulletInstrument<adouble>(startDate, endDate, paymentFrequency, notional, spread, curveManager.curveContext("TEST").index(),
                                                          curveManager.curveContext("TEST").idx(), curveManager.curveContext("TEST").idx());
        };
    };
}  // namespace TestZSpreadConstVisitor

#endif /* B8AC5D97_78E1_4A3A_B0E8_C81B7761D235 */
