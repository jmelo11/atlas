#ifndef C38DCDF6_DEAD_4E60_8C27_7DE662F4D757
#define C38DCDF6_DEAD_4E60_8C27_7DE662F4D757

#include <atlas/data/marketrequest.hpp>
#include <atlas/rates/index/interestrateindex.hpp>
#include <gtest/gtest.h>

using namespace Atlas;

namespace TestFixingVisitor {

    struct Common {
        Date startDate                  = Date(1, Month::January, 2020);
        Date endDate                    = Date(1, Month::January, 2025);
        double notional                 = 1000000.0;
        InterestRate<double> rate       = InterestRate(0.05, Actual360());
        double spread                   = 0.01;
        InterestRateIndex<double> index = InterestRateIndex(Frequency::Semiannual);
        Frequency paymentFrequency      = Frequency::Semiannual;
        Side side                       = Side::Recieve;
        size_t discountIdx              = 0;
        size_t indexIdx                 = 0;
    };

    template <class C>
    inline void testFixing(const C& cashflow) {
        EXPECT_TRUE(cashflow.isFixingSet());
    }
}  // namespace TestFixingVisitor

#endif /* C38DCDF6_DEAD_4E60_8C27_7DE662F4D757 */
