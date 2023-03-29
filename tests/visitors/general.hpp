#ifndef E4EE0D74_1E13_403E_BFAA_A52DB3AD99FF
#define E4EE0D74_1E13_403E_BFAA_A52DB3AD99FF

#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/rates/curvecontextstore.hpp>

using namespace Atlas;

struct FixedInstrumentVars {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    InterestRate rate          = InterestRate(0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    CurveContextStore& store_  = CurveContextStore::instance();
    FixedInstrumentVars() {
        if (!store_.hasContext("TEST")) {
            FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
            RateIndex index("TEST", Frequency::Annual, Actual360());
            store_.createCurveContext("TEST", curveStrategy, index);
        }
    };
};

struct FloatingInstrumentVars {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    double spread              = 0.01;
    CurveContextStore& store_  = CurveContextStore::instance();
    FloatingInstrumentVars() {
        if (!store_.hasContext("TEST")) {
            FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
            RateIndex index("TEST", Frequency::Annual, Actual360());
            store_.createCurveContext("TEST", curveStrategy, index);
        }
    };
};

#endif /* E4EE0D74_1E13_403E_BFAA_A52DB3AD99FF */
