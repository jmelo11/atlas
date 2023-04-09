#ifndef E4EE0D74_1E13_403E_BFAA_A52DB3AD99FF
#define E4EE0D74_1E13_403E_BFAA_A52DB3AD99FF

#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/fundation/marketstore.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>

using namespace Atlas;

template <typename adouble>
struct FixedInstrumentVars {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    adouble rateValue          = 0.03;
    InterestRate<adouble> rate = InterestRate(rateValue, Actual360(), Compounding::Simple, Frequency::Annual);
    MarketStore<adouble> store_;
    FixedInstrumentVars() {
        FlatForwardStrategy<adouble> curveStrategy(startDate, rateValue, Actual360(), Compounding::Simple, Frequency::Annual);
        YieldTermStructure<adouble> curve(std::make_unique<FlatForwardStrategy<adouble>>(curveStrategy));
        RateIndex index("TEST", Frequency::Annual, Actual360());
        store_.createCurveContext("TEST", curve, index);
    };
};

template <typename adouble>
struct FloatingInstrumentVars {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    adouble spread             = 0.01;
    adouble rateValue          = 0.03;
    MarketStore<adouble> store_;
    FloatingInstrumentVars() {
        FlatForwardStrategy<adouble> curveStrategy(startDate, rateValue, Actual360(), Compounding::Simple, Frequency::Annual);
        YieldTermStructure<adouble> curve(std::make_unique<FlatForwardStrategy<adouble>>(curveStrategy));
        RateIndex index("TEST", Frequency::Annual, Actual360());
        store_.createCurveContext("TEST", curve, index);
    };
};

#endif /* E4EE0D74_1E13_403E_BFAA_A52DB3AD99FF */
