#include "general.hpp"
#include <atlas/instruments/fixedrate/fixedrateequalredemptioninstrument.hpp>

TEST(Instrument, FixedRateEqualRedemptionInstrument) {
    TestSetup<double> vars;
    FixedRateEqualRedemptionInstrument<double> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.atlasRate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testStructure<FixedRateEqualRedemptionInstrument<double>, double>(prod, schedule, PaymentStructure::EqualRedemptions);
    testInterest<double>(prod, schedule, vars.atlasRate);
    testChangeCurrency<FixedRateEqualRedemptionInstrument<double>, double>(prod);
};

TEST(Instrument, FixedRateEqualRedemptionInstrumentDual) {
    TestSetup<dual> vars;
    FixedRateEqualRedemptionInstrument<dual> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.atlasRate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testStructure<FixedRateEqualRedemptionInstrument<dual>, dual>(prod, schedule, PaymentStructure::EqualRedemptions);
    testInterest<dual>(prod, schedule, vars.atlasRate);
    testChangeCurrency<FixedRateEqualRedemptionInstrument<dual>, dual>(prod);
};
