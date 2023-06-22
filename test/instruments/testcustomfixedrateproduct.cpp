#include "general.hpp"
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <numeric>

TEST(Instrument, CustomFixedRateInstrument) {
    TestSetup<double> vars;
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);

    std::vector<double> redemptionAmounts(schedule.dates().size() - 1, 50);  // constant redemptions
    auto notional = std::reduce(redemptionAmounts.begin(), redemptionAmounts.end());
    CustomFixedRateInstrument<double> prod(schedule.dates(), redemptionAmounts, vars.atlasRate);
    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();
    EXPECT_EQ(coupons.size(), 2);
    EXPECT_EQ(redemptions.size(), 2);

    for (const auto& redemption : redemptions) { EXPECT_EQ(redemption.amount(), notional / 2); }

    testChangeCurrency<CustomFixedRateInstrument<double>, double>(prod);
}

TEST(Instrument, CustomFixedRateInstrumentDual) {
    TestSetup<dual> vars;
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);

    std::vector<double> redemptionAmounts(schedule.dates().size() - 1, 50);  // constant redemptions
    auto notional = std::reduce(redemptionAmounts.begin(), redemptionAmounts.end());
    CustomFixedRateInstrument<dual> prod(schedule.dates(), redemptionAmounts, vars.atlasRate);
    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();
    EXPECT_EQ(coupons.size(), 2);
    EXPECT_EQ(redemptions.size(), 2);

    for (const auto& redemption : redemptions) { EXPECT_EQ(val(redemption.amount()), notional / 2); }

    testChangeCurrency<CustomFixedRateInstrument<dual>, dual>(prod);
}