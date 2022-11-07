#include "pch.hpp"
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/fixedbulletproduct.hpp>

using namespace Atlas;

TEST(TestFixedBullet, FixedBulletProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional = 100;
    QuantLib::InterestRate rate(0.03, QuantLib::Actual360(), QuantLib::Simple, QuantLib::Annual);
    FixedBulletProduct prod(startDate, endDate, notional, rate);

    const Leg& tmp = prod.constLeg(1);

    EXPECT_EQ(tmp.discountCurve(), "undefined");
    EXPECT_EQ(tmp.forecastCurve(), "undefined");

    for (const auto& cashflow : tmp.cashflows()) {
        FixedRateCoupon* ptr = std::dynamic_pointer_cast<FixedRateCoupon>(cashflow);
        if (ptr) {
            double yf       = rate.dayCounter().yearFraction(ptr->startDate(), ptr->endDate());
            double interest = yf * notional * rate.rate();
            EXPECT_FLOAT_EQ(interest, ptr->amount());
        } else {
            Redemption* tmpPtr = std::dynamic_pointer_cast<Redemption>(cashflow);
            EXPECT_EQ(tmpPtr->amount(), notional);
            EXPECT_EQ(tmpPtr->date(), endDate);
        }
    }
}