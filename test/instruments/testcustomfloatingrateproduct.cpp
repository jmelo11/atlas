#include "general.hpp"
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <numeric>

TEST(Instrument, CustomFloatingRateInstrument) {
    TestSetup<double> vars;

    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    std::vector<double> redemptionAmounts(schedule.dates().size() - 1, 50);  // constant redemptions

    auto notional = std::reduce(redemptionAmounts.begin(), redemptionAmounts.end());

    auto& context = vars.store.curveManager().curveContext("TEST");
    auto& index   = context.index();
    CustomFloatingRateInstrument<double> prod(schedule.dates(), redemptionAmounts, vars.spread, index);

    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();
    EXPECT_EQ(coupons.size(), 2);
    EXPECT_EQ(redemptions.size(), 2);

    for (const auto& redemption : redemptions) { EXPECT_EQ(redemption.amount(), notional / 2); }

    testChangeCurrency<CustomFloatingRateInstrument<double>, double>(prod);
}

TEST(Instrument, CustomFloatingRateInstrumentDual) {
    TestSetup<dual> vars;

    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    std::vector<double> redemptionAmounts(schedule.dates().size() - 1, 50);  // constant redemptions

    auto notional = std::reduce(redemptionAmounts.begin(), redemptionAmounts.end());

    auto& context = vars.store.curveManager().curveContext("TEST");
    auto& index   = context.index();
    CustomFloatingRateInstrument<dual> prod(schedule.dates(), redemptionAmounts, vars.spread, index);

    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();
    EXPECT_EQ(coupons.size(), 2);
    EXPECT_EQ(redemptions.size(), 2);

    for (const auto& redemption : redemptions) { EXPECT_EQ(val(redemption.amount()), notional / 2); }

    testChangeCurrency<CustomFloatingRateInstrument<dual>, dual>(prod);
}