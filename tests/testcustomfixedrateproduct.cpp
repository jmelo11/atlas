
#include "pch.hpp"
#include <ql/time/schedule.hpp>
#include <atlas/instruments/fixedrate/customfixedrateproduct.hpp>
#include <numeric>

using namespace Atlas;

TEST(Products, CustomFixedRateProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);

    QuantLib::Frequency freq    = QuantLib::Frequency::Semiannual;
    QuantLib::Schedule schedule = QuantLib::MakeSchedule().from(startDate).to(endDate).withFrequency(freq);

    QuantLib::DayCounter dayCounter = QuantLib::Actual360();
    QuantLib::InterestRate rate(0.03, dayCounter, QuantLib::Compounding::Simple, QuantLib::Frequency::Annual);

    std::vector<double> redemptionAmounts(schedule.dates().size() - 1, 50);  // constant redemptions
    auto notional = std::reduce(redemptionAmounts.begin(), redemptionAmounts.end());
    CustomFixedRateProduct prod(schedule.dates(), redemptionAmounts, rate);

    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();

    EXPECT_EQ(leg.discountCurve(), "undefined");

    EXPECT_EQ(coupons.size(), 2);
    EXPECT_EQ(redemptions.size(), 2);

    EXPECT_EQ(coupons.at(0).amount(), notional * (rate.compoundFactor(coupons.at(0).startDate(), coupons.at(0).endDate()) - 1)); 
    EXPECT_EQ(coupons.at(1).amount(), notional/2 * (rate.compoundFactor(coupons.at(1).startDate(), coupons.at(1).endDate()) - 1)); 
    for (const auto& redemption : redemptions) { EXPECT_EQ(redemption.amount(), notional/2); }
}