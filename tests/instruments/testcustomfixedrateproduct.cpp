
#include "../pch.hpp"
#include "general.hpp"
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <numeric>

TEST(Instruments, CustomFixedRateInstrument) {
    FixedInstrumentVars vars;
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);

    std::vector<double> redemptionAmounts(schedule.dates().size() - 1, 50);  // constant redemptions
    auto notional = std::reduce(redemptionAmounts.begin(), redemptionAmounts.end());
    CustomFixedRateInstrument prod(schedule.dates(), redemptionAmounts, vars.rate);
    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();
    EXPECT_EQ(coupons.size(), 2);
    EXPECT_EQ(redemptions.size(), 2);

    for (const auto& redemption : redemptions) { EXPECT_EQ(redemption.amount(), notional / 2); }
}