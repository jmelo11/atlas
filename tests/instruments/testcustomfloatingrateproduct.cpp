
#include "../pch.hpp"
#include "general.hpp"
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <numeric>

TEST(Instrument, CustomFloatingRateInstrument) {
    FloatingInstrumentVars<double> vars;

    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    std::vector<double> redemptionAmounts(schedule.dates().size() - 1, 50);  // constant redemptions

    auto notional = std::reduce(redemptionAmounts.begin(), redemptionAmounts.end());

    CustomFloatingRateInstrument<double> prod(schedule.dates(), redemptionAmounts, vars.spread, vars.store_.curveContext("TEST"));

    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();
    EXPECT_EQ(coupons.size(), 2);
    EXPECT_EQ(redemptions.size(), 2);

    for (const auto& redemption : redemptions) { EXPECT_EQ(redemption.amount(), notional / 2); }
}

TEST(Instrument, CustomFloatingRateInstrumentDual) {
    FloatingInstrumentVars<dual> vars;

    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    std::vector<double> redemptionAmounts(schedule.dates().size() - 1, 50);  // constant redemptions

    auto notional = std::reduce(redemptionAmounts.begin(), redemptionAmounts.end());

    CustomFloatingRateInstrument<dual> prod(schedule.dates(), redemptionAmounts, vars.spread, vars.store_.curveContext("TEST"));

    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();
    EXPECT_EQ(coupons.size(), 2);
    EXPECT_EQ(redemptions.size(), 2);

    for (const auto& redemption : redemptions) { EXPECT_EQ(redemption.amount().val, notional / 2); }
}