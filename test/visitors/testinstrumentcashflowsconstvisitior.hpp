#ifndef D97CA93A_E07E_4918_9640_A5EC6660198D
#define D97CA93A_E07E_4918_9640_A5EC6660198D

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
#include <atlas/rates/index/interestrateindex.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/rates/yieldtermstructure/zeroratecurve.hpp>
#include <gtest/gtest.h>

using namespace Atlas;

namespace TestInstrumentCashflowsConstVisitor {
    struct Common {
        Date startDate  = Date(1, Month::January, 2020);
        Date endDate    = Date(1, Month::January, 2025);
        double notional = 100.0;

        // rate parameters
        DayCounter dayCounter   = Actual360();
        Frequency frequency     = Frequency::Annual;
        Compounding compounding = Compounding::Simple;
        double rate             = 0.03;

        double spread                   = 0.0;
        InterestRateIndex<double> index = InterestRateIndex(Frequency::Semiannual);
        Frequency paymentFrequency      = Frequency::Semiannual;
        Side side                       = Side::Recieve;
        size_t discountIdx              = 0;
        size_t indexIdx                 = 0;
    };
}  // namespace TestInstrumentCashflowsConstVisitor

#endif /* D97CA93A_E07E_4918_9640_A5EC6660198D */
