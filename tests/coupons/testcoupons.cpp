#include "../pch.hpp"
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/fundation/marketstore.hpp>
#include <atlas/rates/rateindex.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>

using namespace Atlas;

template <typename adouble>
struct FixedCouponVars {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    double notional            = 100;
    adouble rateValue          = 0.03;
    InterestRate<adouble> rate = InterestRate<adouble>(rateValue, Actual360(), Compounding::Simple, Frequency::Annual);

    MarketStore<adouble> store_;
    FixedCouponVars() {
        FlatForwardStrategy<adouble> curveStrategy(startDate, rateValue, Actual360(), Compounding::Simple, Frequency::Annual);
        YieldTermStructure<adouble> curve(std::make_unique<FlatForwardStrategy<adouble>>(curveStrategy));
        RateIndex index("TEST", Frequency::Annual, Actual360());
        store_.createCurveContext("TEST", curve, index);
    };
};

template <typename adouble>
struct FloatingCouponVars {
    Date startDate    = Date(1, Month::Aug, 2020);
    Date endDate      = Date(1, Month::Aug, 2021);
    double notional   = 100;
    adouble spread    = 0.01;
    adouble rateValue = 0.03;
    MarketStore<adouble> store_;
    FloatingCouponVars() {
        FlatForwardStrategy<adouble> curveStrategy(startDate, rateValue, Actual360(), Compounding::Simple, Frequency::Annual);
        YieldTermStructure<adouble> curve(std::make_unique<FlatForwardStrategy<adouble>>(curveStrategy));
        RateIndex index("TEST", Frequency::Annual, Actual360());
        store_.createCurveContext("TEST", curve, index);
    };
};

TEST(Coupons, FixedRateCoupon) {
    FixedCouponVars<double> vars;
    FixedRateCoupon<double> coupon(vars.startDate, vars.endDate, vars.notional, vars.rate, vars.store_.curveContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());

    double yf = vars.rate.dayCounter().yearFraction(vars.startDate, vars.endDate);
    EXPECT_FLOAT_EQ(yf * vars.rate.rate() * vars.notional, coupon.accruedAmount(vars.startDate, vars.endDate));
    EXPECT_FLOAT_EQ(yf * vars.rate.rate() * vars.notional, coupon.amount());

    InterestRate<double> rate(0.05, Actual360(), Compounding::Simple, Frequency::Annual);
    coupon.rate(rate);

    EXPECT_FLOAT_EQ(yf * rate.rate() * vars.notional, coupon.amount());
    EXPECT_FLOAT_EQ(yf * rate.rate() * vars.notional, coupon.accruedAmount(vars.startDate, vars.endDate));
}

TEST(Coupons, FloatingRateCoupon) {
    FloatingCouponVars<double> vars;
    FloatingRateCoupon<double> coupon(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store_.curveContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());
    EXPECT_FLOAT_EQ(vars.spread, coupon.spread());
}

TEST(Coupons, FixedRateCouponDual) {
    FixedCouponVars<dual> vars;
    FixedRateCoupon<dual> coupon(vars.startDate, vars.endDate, vars.notional, vars.rate, vars.store_.curveContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());

    double yf = vars.rate.dayCounter().yearFraction(vars.startDate, vars.endDate);
    EXPECT_FLOAT_EQ(yf * vars.rate.rate().val * vars.notional, coupon.accruedAmount(vars.startDate, vars.endDate).val);
    EXPECT_FLOAT_EQ(yf * vars.rate.rate().val * vars.notional, coupon.amount().val);

    InterestRate<dual> rate(0.05, Actual360(), Compounding::Simple, Frequency::Annual);
    coupon.rate(rate);

    EXPECT_FLOAT_EQ(yf * rate.rate().val * vars.notional, coupon.amount().val);
    EXPECT_FLOAT_EQ(yf * rate.rate().val * vars.notional, coupon.accruedAmount(vars.startDate, vars.endDate).val);
}

TEST(Coupons, FloatingRateCouponDual) {
    FloatingCouponVars<dual> vars;
    FloatingRateCoupon<dual> coupon(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store_.curveContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());
    EXPECT_FLOAT_EQ(vars.spread.val, coupon.spread().val);
}