#include "../pch.hpp"
#include <ql/time/daycounters/actual360.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/curves/rateindex.hpp>

using namespace Atlas;

struct FixedCouponVars {
    Date startDate    = Date(1, Month::Aug, 2020);
    Date endDate      = Date(1, Month::Aug, 2021);
    double notional   = 100;
    InterestRate rate = InterestRate(0.03, Actual360(), Compounding::Simple, Frequency::Annual);
};

struct FloatingCouponVars {
    Date startDate  = Date(1, Month::Aug, 2020);
    Date endDate    = Date(1, Month::Aug, 2021);
    double notional = 100;
    double spread   = 0.01;
    size_t index    = 0;
};

TEST(Coupons, FixedRateCoupon) {
    FixedCouponVars vars;
    FixedRateCoupon coupon(vars.startDate, vars.endDate, vars.notional, vars.rate);

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());

    double yf = vars.rate.dayCounter().yearFraction(vars.startDate, vars.endDate);
    EXPECT_FLOAT_EQ(yf * vars.rate.rate() * vars.notional, coupon.accruedAmount(vars.startDate, vars.endDate));
    EXPECT_FLOAT_EQ(yf * vars.rate.rate() * vars.notional, coupon.amount());

    InterestRate rate(0.05, Actual360(), Compounding::Simple, Frequency::Annual);
    coupon.rate(rate);

    EXPECT_FLOAT_EQ(yf * rate.rate() * vars.notional, coupon.amount());
    EXPECT_FLOAT_EQ(yf * rate.rate() * vars.notional, coupon.accruedAmount(vars.startDate, vars.endDate));
}

TEST(Coupons, FloatingRateCoupon) {
    FloatingCouponVars vars;
    FloatingRateCoupon coupon(vars.startDate, vars.endDate, vars.notional, vars.spread);

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());
    EXPECT_FLOAT_EQ(vars.spread, coupon.spread());
}