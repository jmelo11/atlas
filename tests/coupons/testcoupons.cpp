#include "../testsetup.hpp"
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>

using namespace Atlas;

TEST(Coupons, FixedRateCoupon) {
    TestSetup<double> vars;
    FixedRateCoupon<double> coupon(vars.startDate, vars.endDate, vars.notional, vars.atlasRate, vars.store.curveContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());

    double yf = vars.atlasRate.dayCounter().yearFraction(vars.startDate, vars.endDate);
    EXPECT_FLOAT_EQ(yf * vars.atlasRate.rate() * vars.notional, coupon.accruedAmount(vars.startDate, vars.endDate));
    EXPECT_FLOAT_EQ(yf * vars.atlasRate.rate() * vars.notional, coupon.amount());

    coupon.rate(vars.atlasRate);

    EXPECT_FLOAT_EQ(yf * vars.atlasRate.rate() * vars.notional, coupon.amount());
    EXPECT_FLOAT_EQ(yf * vars.atlasRate.rate() * vars.notional, coupon.accruedAmount(vars.startDate, vars.endDate));
}

TEST(Coupons, FloatingRateCoupon) {
    TestSetup<double> vars;
    FloatingRateCoupon<double> coupon(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store.rateIndexContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());
    EXPECT_FLOAT_EQ(vars.spread, coupon.spread());
}

TEST(Coupons, FixedRateCouponDual) {
    TestSetup<dual> vars;
    FixedRateCoupon<dual> coupon(vars.startDate, vars.endDate, vars.notional, vars.atlasRate, vars.store.curveContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());

    double yf = vars.atlasRate.dayCounter().yearFraction(vars.startDate, vars.endDate);
    EXPECT_FLOAT_EQ(yf * val(vars.atlasRate.rate()) * vars.notional, val(coupon.accruedAmount(vars.startDate, vars.endDate)));
    EXPECT_FLOAT_EQ(yf * val(vars.atlasRate.rate()) * vars.notional, val(coupon.amount()));

    coupon.rate(vars.atlasRate);

    EXPECT_FLOAT_EQ(yf * val(vars.atlasRate.rate()) * vars.notional, val(coupon.amount()));
    EXPECT_FLOAT_EQ(yf * val(vars.atlasRate.rate()) * vars.notional, val(coupon.accruedAmount(vars.startDate, vars.endDate)));
}

TEST(Coupons, FloatingRateCouponDual) {
    TestSetup<dual> vars;
    FloatingRateCoupon<dual> coupon(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store.rateIndexContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());
    EXPECT_FLOAT_EQ(val(vars.spread), val(coupon.spread()));
}