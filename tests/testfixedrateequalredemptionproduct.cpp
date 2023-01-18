#include "pch.hpp"
#include <ql/time/daycounters/actual360.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/fixedrate/fixedrateequalredemptionproduct.hpp>

using namespace Atlas;

TEST(Product, FixedRateEqualRedemptionProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional          = 100;
    QuantLib::Frequency freq = QuantLib::Frequency::Monthly;
    QuantLib::InterestRate rate(0.03, QuantLib::Actual360(), QuantLib::Simple, QuantLib::Annual);
    FixedRateEqualRedemptionProduct prod(startDate, endDate, freq, notional, rate);

    const auto& leg         = prod.constLeg();
    const auto& coupons     = leg.constCoupons();
    const auto& redemptions = leg.constRedemptions();

    EXPECT_EQ(leg.discountCurve(), "undefined");

    EXPECT_EQ(coupons.size(), 12);
    EXPECT_EQ(redemptions.size(), 12);

    double outstanding = notional;
    for (size_t i = 0; i < coupons.size(); ++i) {
        auto& coupon = coupons[i];
        auto& redemption = redemptions[i];
        EXPECT_NEAR(coupon.amount(), outstanding * (rate.compoundFactor(coupon.startDate(), coupon.endDate()) - 1), 0.001);
        EXPECT_NEAR(redemption.amount(), notional / 12, 0.001);
        outstanding -= redemptions[i].amount();
    }
    
}
