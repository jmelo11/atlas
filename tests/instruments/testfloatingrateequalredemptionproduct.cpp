#include "../pch.hpp"
#include "general.hpp"
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>

TEST(Instrument, FloatingRateEqualRedemptionInstrument) {
    FloatingInstrumentVars<double> vars;
    auto& index = vars.store_.curveContext("TEST").index();
    FloatingRateEqualRedemptionInstrument<double> inst(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store_.curveContext("TEST"));
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(index.fixingFrequency());
    testStructure<FloatingRateEqualRedemptionInstrument<double>, double>(inst, schedule, PaymentStructure::EqualRedemptions);
};

TEST(Instrument, FloatingRateEqualRedemptionInstrumentDual) {
    FloatingInstrumentVars<dual> vars;
    auto& index = vars.store_.curveContext("TEST").index();
    FloatingRateEqualRedemptionInstrument<dual> inst(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store_.curveContext("TEST"));
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(index.fixingFrequency());
    testStructure<FloatingRateEqualRedemptionInstrument<dual>, dual>(inst, schedule, PaymentStructure::EqualRedemptions);
};