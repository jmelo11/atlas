#include "general.hpp"
#include <atlas/instruments/fixedrate/fixedrateequalredemptioninstrument.hpp>

TEST(Instrument, FixedRateEqualRedemptionInstrument) {
    TestSetup<double> vars;
    FixedRateEqualRedemptionInstrument<double> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.atlasRate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testRegularStructure(prod, schedule.dates(), PaymentStructure::EqualRedemptions);
};
