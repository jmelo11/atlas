#include "pch.hpp"
#include <ql/time/daycounters/actual360.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/fixedrate/fixedbulletproduct.hpp>

using namespace Atlas;

TEST(Product, FixedBulletProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional          = 100;
    QuantLib::Frequency freq = QuantLib::Frequency::Semiannual;
    QuantLib::InterestRate rate(0.03, QuantLib::Actual360(), QuantLib::Simple, QuantLib::Annual);
    FixedBulletProduct prod(startDate, endDate, freq, notional, rate);

    const auto& leg         = prod.constLeg();
    const auto& coupons     = leg.constCoupons();
    const auto& redemptions = leg.constRedemptions();

    EXPECT_EQ(leg.discountCurve(), "undefined");

    EXPECT_EQ(coupons.size(), 2);
    EXPECT_EQ(redemptions.size(), 1);

    for (const auto& coupon : coupons) {
        EXPECT_EQ(coupon.amount(),
                  notional * (rate.compoundFactor(coupon.startDate(), coupon.endDate()) - 1));
    }
    for (const auto& redemption : redemptions) { EXPECT_EQ(redemption.amount(), notional); }
}