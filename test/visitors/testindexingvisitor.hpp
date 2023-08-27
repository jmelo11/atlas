#ifndef AED7AD89_488D_42B2_81DF_61CFE815C125
#define AED7AD89_488D_42B2_81DF_61CFE815C125

#include <atlas/cashflows/cashflowstreammixins.hpp>
#include <atlas/data/marketrequest.hpp>
#include <atlas/rates/index/index.hpp>
#include <gtest/gtest.h>

using namespace Atlas;

namespace TestIndexingVisitor {

    struct Common {
        Date startDate             = Date(1, Month::January, 2020);
        Date endDate               = Date(1, Month::January, 2025);
        double notional            = 1000000.0;
        InterestRate<double> rate  = InterestRate(0.05, Actual360());
        double spread              = 0.01;
        Index<double> index        = Index();
        Frequency paymentFrequency = Frequency::Semiannual;
        Side side                  = Side::Recieve;
        size_t discountIdx         = 0;
        size_t indexIdx            = 0;
    };

    template <class CS>
    inline void testindexer(const CS& cashflows, const MarketRequest& request) {
        std::set<Date> dfDates;
        std::set<Date> fxDates;
        std::set<Date> fwdDates;

        if constexpr (std::is_base_of_v<DisbursementStreamMixin<double>, CS>) {
            for (const auto& cf : cashflows.disbursements()) {
                EXPECT_NE(cf.dfIdx(), SIZE_MAX);
                EXPECT_NE(cf.fxIdx(), SIZE_MAX);
                dfDates.insert(cf.paymentDate());
                if (cf.applyCurrency()) fxDates.insert(cf.paymentDate());
            }
        }
        if constexpr (std::is_base_of_v<RedemptionStreamMixin<double>, CS>) {
            for (const auto& cf : cashflows.redemptions()) {
                EXPECT_NE(cf.dfIdx(), SIZE_MAX);
                EXPECT_NE(cf.fxIdx(), SIZE_MAX);
                dfDates.insert(cf.paymentDate());
                if (cf.applyCurrency()) fxDates.insert(cf.paymentDate());
            }
        }
        if constexpr (std::is_base_of_v<FixedRateCouponStreamMixin<double>, CS>) {
            for (const auto& cf : cashflows.fixedRateCoupons()) {
                EXPECT_NE(cf.dfIdx(), SIZE_MAX);
                EXPECT_NE(cf.fxIdx(), SIZE_MAX);
                dfDates.insert(cf.paymentDate());
                if (cf.applyCurrency()) fxDates.insert(cf.paymentDate());
            }
        }
        if constexpr (std::is_base_of_v<FloatingRateCouponStreamMixin<double>, CS>) {
            for (const auto& cf : cashflows.floatingRateCoupons()) {
                EXPECT_NE(cf.dfIdx(), SIZE_MAX);
                EXPECT_NE(cf.fxIdx(), SIZE_MAX);
                EXPECT_NE(cf.fwdIdx(), SIZE_MAX);
                dfDates.insert(cf.paymentDate());
                if (cf.applyCurrency()) fxDates.insert(cf.paymentDate());
                fwdDates.insert(cf.paymentDate());
            }
        }

        EXPECT_EQ(dfDates.size(), request.dfs.size());
        EXPECT_EQ(fwdDates.size(), request.fwds.size());
        EXPECT_EQ(fxDates.size(), request.fxs.size() - 1);
    }

}  // namespace TestIndexingVisitor

#endif /* AED7AD89_488D_42B2_81DF_61CFE815C125 */
