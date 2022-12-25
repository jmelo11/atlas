
#include "pch.hpp"
#include <ql/time/schedule.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateproduct.hpp>
#include <numeric>

using namespace Atlas;

TEST(Products, CustomFloatingRateProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);

    QuantLib::Frequency freq    = QuantLib::Frequency::Semiannual;
    QuantLib::Schedule schedule = QuantLib::MakeSchedule().from(startDate).to(endDate).withFrequency(freq);

    double spread = 0.01;
    LIBOR6M index;

    std::vector<double> redemptionAmounts(schedule.dates().size() - 1, 50);  // constant redemptions
    auto notional = std::reduce(redemptionAmounts.begin(), redemptionAmounts.end());
    CustomFloatingRateProduct prod(schedule.dates(), redemptionAmounts, spread, index);

    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();

    EXPECT_EQ(leg.discountCurve(), "undefined");
    EXPECT_EQ(leg.forecastCurve(), index.name());

    EXPECT_EQ(coupons.size(), 2);
    EXPECT_EQ(redemptions.size(), 2);

    for (const auto& redemption : redemptions) { EXPECT_EQ(redemption.amount(), notional / 2); }
}