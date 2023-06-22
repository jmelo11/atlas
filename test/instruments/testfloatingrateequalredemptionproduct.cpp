#include "general.hpp"
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>

TEST(Instrument, FloatingRateEqualRedemptionInstrument) {
    TestSetup<double> vars;
    auto& context = vars.store.rateIndexContext("TEST");
    auto& index   = context.object();
    FloatingRateEqualRedemptionInstrument<double> inst(vars.startDate, vars.endDate, vars.notional, vars.spread, context);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(index.fixingFrequency());
    testStructure<FloatingRateEqualRedemptionInstrument<double>, double>(inst, schedule, PaymentStructure::EqualRedemptions);
    testChangeCurrency<FloatingRateEqualRedemptionInstrument<double>, double>(inst);
};

TEST(Instrument, FloatingRateEqualRedemptionInstrumentDual) {
    TestSetup<dual> vars;
    auto& context = vars.store.rateIndexContext("TEST");
    auto& index   = context.object();
    FloatingRateEqualRedemptionInstrument<dual> inst(vars.startDate, vars.endDate, vars.notional, vars.spread, context);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(index.fixingFrequency());
    testStructure<FloatingRateEqualRedemptionInstrument<dual>, dual>(inst, schedule, PaymentStructure::EqualRedemptions);
    testChangeCurrency<FloatingRateEqualRedemptionInstrument<dual>, dual>(inst);
};