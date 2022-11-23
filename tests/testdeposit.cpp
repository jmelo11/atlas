#include "pch.hpp"
#include <ql/time/daycounters/actual360.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/fixedrate/deposit.hpp>

using namespace Atlas;

TEST(Product, Deposit) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional = 100;
    QuantLib::InterestRate rate(0.03, QuantLib::Actual360(), QuantLib::Simple, QuantLib::Annual);
    Deposit prod(startDate, endDate, notional, rate);

    const auto& leg         = prod.constLeg();
    const auto& coupons     = leg.constCoupons();
    const auto& redemptions = leg.constRedemptions();

    EXPECT_EQ(leg.discountCurve(), "undefined");
    EXPECT_EQ(coupons.size(), 1);
    EXPECT_EQ(redemptions.size(), 1);

    const auto& coupon = coupons[0];
    EXPECT_EQ(coupon.amount(), notional * (rate.compoundFactor(startDate, endDate) - 1));
    EXPECT_EQ(coupon.startDate(), startDate);
    EXPECT_EQ(coupon.endDate(), endDate);
    EXPECT_EQ(coupon.rate(), rate);

    const auto& redemption = redemptions[0];

    EXPECT_EQ(redemption.amount(), 100.0);
    EXPECT_EQ(redemption.paymentDate(), endDate);
}