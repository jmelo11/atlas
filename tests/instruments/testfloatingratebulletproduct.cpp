#include "../pch.hpp"
#include "general.hpp"
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>

TEST(Instrument, FloatingRateBulletInstrument) {
    FloatingInstrumentVars<double> vars;
    auto& index = vars.store_.curveContext("TEST").index();
    FloatingRateBulletInstrument<double> inst(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store_.curveContext("TEST"));
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(index.fixingFrequency());
    testStructure<FloatingRateBulletInstrument<double>, double>(inst, schedule, PaymentStructure::BulletOrZero);
}

TEST(Instrument, FloatingRateBulletInstrumentDual) {
    FloatingInstrumentVars<dual> vars;
    auto& index = vars.store_.curveContext("TEST").index();
    FloatingRateBulletInstrument<dual> inst(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store_.curveContext("TEST"));
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(index.fixingFrequency());
    testStructure<FloatingRateBulletInstrument<dual>, dual>(inst, schedule, PaymentStructure::BulletOrZero);
}