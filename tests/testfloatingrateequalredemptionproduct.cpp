#include "pch.hpp"
#include <ql/time/schedule.hpp>
#include <atlas/curves/rateindex.hpp>
#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptionproduct.hpp>

using namespace Atlas;

TEST(Product, FloatingRateEqualRedemptionProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional = 100;
    double spread   = 0.01;
    double fixing   = 0.03;
    LIBOR6M index;
    FloatingRateEqualRedemptionProduct prod(startDate, endDate, notional, spread, index);

    QuantLib::Schedule schedule = QuantLib::MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());

    const auto& leg         = prod.constLeg();
    const auto& coupons     = leg.constCoupons();
    const auto& redemptions = leg.constRedemptions();

    EXPECT_EQ(leg.discountCurve(), "undefined");

    EXPECT_EQ(coupons.size(), 2);
    EXPECT_EQ(redemptions.size(), 2);

    
    for (const auto& redemption : redemptions) { EXPECT_EQ(redemption.amount(), notional / 2); }
}