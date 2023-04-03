#include "../pch.hpp"
#include "general.hpp"
#include <atlas/instruments/fixedrate/fixedrateequalredemptioninstrument.hpp>

TEST(Instrument, FixedRateEqualRedemptionInstrument) {
    FixedInstrumentVars<double> vars;
    FixedRateEqualRedemptionInstrument<double> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testStructure<FixedRateEqualRedemptionInstrument<double>, double>(prod, schedule, PaymentStructure::EqualRedemptions);
    testInterest<double>(prod, schedule, vars.rate);
};

TEST(Instrument, FixedRateEqualRedemptionInstrumentDual) {
    FixedInstrumentVars<dual> vars;
    FixedRateEqualRedemptionInstrument<dual> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testStructure<FixedRateEqualRedemptionInstrument<dual>, dual>(prod, schedule, PaymentStructure::EqualRedemptions);
    testInterest<dual>(prod, schedule, vars.rate);
};
