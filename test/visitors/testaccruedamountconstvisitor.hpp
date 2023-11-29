#ifndef DD0673E1_249B_4A36_8B74_563B6411C97D
#define DD0673E1_249B_4A36_8B74_563B6411C97D

#include <atlas/data/marketrequest.hpp>
#include <atlas/rates/index/interestrateindex.hpp>
#include <gtest/gtest.h>
#include <atlas/cashflows/cashflowstreammixins.hpp>


using namespace Atlas;

namespace TestAccruedAmountConstVisitor {
    struct Common {
        Date startDate                  = Date(1, Month::January, 2020);
        Date endDate                    = Date(1, Month::January, 2025);
        double notional                 = 1000000.0;
        InterestRate<double> rate       = InterestRate(0.05, Actual360());
        double spread                   = 0.01;
        InterestRateIndex<double> index = InterestRateIndex(Frequency::Semiannual);
        Frequency paymentFrequency      = Frequency::Semiannual;
        Side side                       = Side::Receive;
        size_t discountIdx              = 0;
        size_t indexIdx                 = 0;
    };

    template <class CS>
    inline double calculateAccrual(const CS& cashflows, const Date& startDate, const Date& endDate) {
        double acc = 0.0;
        if constexpr (std::is_base_of_v<FixedRateCouponStreamMixin<double>, CS>) {
            for (const auto& cf : cashflows.fixedRateCoupons()) { acc += cf.accruedAmount(startDate, endDate); }
        } else if constexpr (std::is_base_of_v<FloatingRateCouponStreamMixin<double>, CS>) {
            for (const auto& cf : cashflows.floatingRateCoupons()) { acc += cf.accruedAmount(startDate, endDate); }
        }
        return acc;
    }
}  // namespace TestAccruedAmountConstVisitor

#endif /* DD0673E1_249B_4A36_8B74_563B6411C97D */
