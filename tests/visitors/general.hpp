#ifndef E4EE0D74_1E13_403E_BFAA_A52DB3AD99FF
#define E4EE0D74_1E13_403E_BFAA_A52DB3AD99FF

#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>

using namespace Atlas;

struct FixedInstrumentVars {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    InterestRate rate          = InterestRate(0.03, Actual360(), Compounding::Simple, Frequency::Annual);
};

struct FloatingInstrumentVars {
    Date startDate                = Date(1, Month::Aug, 2020);
    Date endDate                  = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency    = Frequency::Semiannual;
    double notional               = 100;
    double spread                 = 0.01;
    RateIndexConfiguration config = RateIndexConfiguration("test", Frequency::Semiannual);
};

#endif /* E4EE0D74_1E13_403E_BFAA_A52DB3AD99FF */
