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
    EXPECT_NEAR(yf * vars.atlasRate.rate() * vars.notional, coupon.accruedAmount(vars.startDate, vars.endDate), 1e-6);
    EXPECT_NEAR(yf * vars.atlasRate.rate() * vars.notional, coupon.amount(), 1e-6);

    coupon.rate(vars.atlasRate);

    EXPECT_NEAR(yf * vars.atlasRate.rate() * vars.notional, coupon.amount(), 1e-6);
    EXPECT_NEAR(yf * vars.atlasRate.rate() * vars.notional, coupon.accruedAmount(vars.startDate, vars.endDate), 1e-6);
}

TEST(Coupons, FloatingRateCoupon) {
    TestSetup<double> vars;
    FloatingRateCoupon<double> coupon(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store.rateIndexContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());
    EXPECT_NEAR(vars.spread, coupon.spread(), 1e-6);
}

TEST(Coupons, FixedRateCouponDual) {
    TestSetup<dual> vars;
    FixedRateCoupon<dual> coupon(vars.startDate, vars.endDate, vars.notional, vars.atlasRate, vars.store.curveContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());

    double yf = vars.atlasRate.dayCounter().yearFraction(vars.startDate, vars.endDate);
    EXPECT_NEAR(yf * val(vars.atlasRate.rate()) * vars.notional, val(coupon.accruedAmount(vars.startDate, vars.endDate)), 1e-6);
    EXPECT_NEAR(yf * val(vars.atlasRate.rate()) * vars.notional, val(coupon.amount()), 1e-6);

    coupon.rate(vars.atlasRate);

    EXPECT_NEAR(yf * val(vars.atlasRate.rate()) * vars.notional, val(coupon.amount()), 1e-6);
    EXPECT_NEAR(yf * val(vars.atlasRate.rate()) * vars.notional, val(coupon.accruedAmount(vars.startDate, vars.endDate)), 1e-6);
}

TEST(Coupons, FloatingRateCouponDual) {
    TestSetup<dual> vars;
    FloatingRateCoupon<dual> coupon(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store.rateIndexContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());
    EXPECT_NEAR(val(vars.spread), val(coupon.spread()), 1e-6);
}