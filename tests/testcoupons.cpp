#include "pch.hpp"
#include <ql/time/daycounters/actual360.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/curves/rateindex.hpp>

using namespace Atlas;

TEST(Coupons, FixedCouponTests) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional = 100;
    QuantLib::InterestRate rate(0.03, QuantLib::Actual360(), QuantLib::Simple, QuantLib::Annual);
    FixedRateCoupon coupon(startDate, endDate, notional, rate);

    EXPECT_EQ(startDate, coupon.startDate());
    EXPECT_EQ(endDate, coupon.endDate());
    EXPECT_EQ(endDate, coupon.paymentDate());

    double yf = rate.dayCounter().yearFraction(startDate, endDate);
    EXPECT_FLOAT_EQ(yf * rate.rate() * notional, coupon.accruedAmount(startDate, endDate));
    EXPECT_FLOAT_EQ(yf * rate.rate() * notional, coupon.amount());

    QuantLib::InterestRate rate2(0.05, QuantLib::Actual360(), QuantLib::Simple, QuantLib::Annual);
    coupon.rate(rate2);

    EXPECT_FLOAT_EQ(yf * rate2.rate() * notional, coupon.amount());
    EXPECT_FLOAT_EQ(yf * rate2.rate() * notional, coupon.accruedAmount(startDate, endDate));
}

TEST(Coupons, FloatingCouponTests) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional = 100;
    LIBOR6M index;
    double fixing = 0.03;
    double spread = 0.01;
    FloatingRateCoupon coupon(startDate, endDate, notional, spread, index);

    EXPECT_EQ(startDate, coupon.startDate());
    EXPECT_EQ(endDate, coupon.endDate());
    EXPECT_EQ(endDate, coupon.paymentDate());
    EXPECT_FLOAT_EQ(spread, coupon.spread());

    coupon.fwdIdx(0);
    coupon.fixing(fixing);

    double yf = index.dayCounter().yearFraction(startDate, endDate);
    EXPECT_FLOAT_EQ(yf * (fixing + spread) * notional, coupon.accruedAmount(startDate, endDate));

    double spread2 = 0.02;
    coupon.spread(spread2);
    EXPECT_FLOAT_EQ(yf * (fixing + spread2) * notional, coupon.accruedAmount(startDate, endDate));    
}