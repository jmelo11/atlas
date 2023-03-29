#include "../pch.hpp"
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/curves/rateindex.hpp>
#include <atlas/rates/curvecontextstore.hpp>

using namespace Atlas;

struct FixedCouponVars {
    Date startDate    = Date(1, Month::Aug, 2020);
    Date endDate      = Date(1, Month::Aug, 2021);
    double notional   = 100;
    InterestRate rate = InterestRate(0.03, Actual360(), Compounding::Simple, Frequency::Annual);

    CurveContextStore& store_ = CurveContextStore::instance();
    FixedCouponVars() {
        if (!store_.hasContext("TEST")) {
            FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
            RateIndex index("TEST", Frequency::Annual, Actual360());
            store_.createCurveContext("TEST", curveStrategy, index);
        }
    };
};

struct FloatingCouponVars {
    Date startDate  = Date(1, Month::Aug, 2020);
    Date endDate    = Date(1, Month::Aug, 2021);
    double notional = 100;
    double spread   = 0.01;

    CurveContextStore& store_ = CurveContextStore::instance();
    FloatingCouponVars() {
        if (!store_.hasContext("TEST")) {
            FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
            RateIndex index("TEST", Frequency::Annual, Actual360());
            store_.createCurveContext("TEST", curveStrategy, index);
        }
    };
};

TEST(Coupons, FixedRateCoupon) {
    FixedCouponVars vars;
    FixedRateCoupon coupon(vars.startDate, vars.endDate, vars.notional, vars.rate, vars.store_.at("TEST"));

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
    FloatingRateCoupon coupon(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store_.at("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());
    EXPECT_FLOAT_EQ(vars.spread, coupon.spread());
}