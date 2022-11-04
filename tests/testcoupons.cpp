#include "pch.hpp"
#include <ql/time/daycounters/actual360.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>

using namespace Atlas;

TEST(FixedCouponTests, Coupons) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional = 100;
    QuantLib::InterestRate rate(0.03, QuantLib::Actual360(), QuantLib::Simple, QuantLib::Annual);
    FixedRateCoupon coupon(startDate, endDate, notional, rate);

    EXPECT_EQ(startDate, coupon.startDate());
    EXPECT_EQ(endDate, coupon.endDate());
    EXPECT_EQ(endDate, coupon.date());

    double yf = rate.dayCounter().yearFraction(startDate, endDate);
    EXPECT_FLOAT_EQ(yf * rate.rate() * notional, coupon.accruedAmount(startDate, endDate));
    EXPECT_FLOAT_EQ(yf * rate.rate() * notional, coupon.amount());
}

TEST(FloatingCouponTests, Coupon) {}