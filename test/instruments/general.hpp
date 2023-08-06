#ifndef A48CDCC5_BB81_4A85_AA59_E7D536EFAC4B
#define A48CDCC5_BB81_4A85_AA59_E7D536EFAC4B

#include "../testsetup.hpp"
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/fundation/marketstore.hpp>
#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>

using namespace Atlas;

enum PaymentStructure { BulletOrZero, EqualRedemptions };

struct Params {
    double amount;
    double notional;
    Date startDate;
    Date endDate;
};

template <class C>
void inline testCashflow(const C& cashflow, Params params) {
    EXPECT_EQ(cashflow.amount(), params.amount);
    EXPECT_EQ(cashflow.paymentDate(), params.endDate);
    if constexpr (std::is_base_of_v<Coupon<double>, C>) {
        EXPECT_EQ(cashflow.notional(), params.notional);
        EXPECT_EQ(cashflow.startDate(), params.startDate);
    }
}

template <class T>
void inline testRegularStructure(const T& instrument, const std::vector<Date>& dates, PaymentStructure structure) {
    auto& cashflows = instrument.cashflows();
    if constexpr (std::is_base_of_v<FixedRateInstrument<double>, T>) { EXPECT_EQ(cashflows.fixedRateCouponCount(), dates.size() - 1); }
    if constexpr (std::is_base_of_v<FloatingRateInstrument<double>, T>) { EXPECT_EQ(cashflows.floatingRateCouponCount(), dates.size() - 1); }

    switch (structure) {
        case PaymentStructure::BulletOrZero:
            EXPECT_EQ(cashflows.redemptionCount(), 1);
            EXPECT_EQ(cashflows.disbursementCount(), 1);
            testCashflow(cashflows.redemption(0), {instrument.notional(), instrument.notional(), instrument.endDate(), instrument.endDate()});
            testCashflow(cashflows.disbursement(0), {instrument.notional(), instrument.notional(), instrument.startDate(), instrument.startDate()});
            break;
        case PaymentStructure::EqualRedemptions:
            EXPECT_EQ(cashflows.redemptionCount(), dates.size() - 1);
            EXPECT_EQ(cashflows.disbursementCount(), 1);
            for (size_t i = 0; i < dates.size() - 1; ++i) {
                Params params = {instrument.notional() / (dates.size() - 1), instrument.notional(), dates[i+1], dates[i+1]};
                testCashflow(cashflows.redemption(i), params);
            }
            break;
        default:
            break;
    }
};

#endif /* A48CDCC5_BB81_4A85_AA59_E7D536EFAC4B */
