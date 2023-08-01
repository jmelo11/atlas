#include "general.hpp"
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>

TEST(Instrument, FloatingRateBulletInstrument) {
    TestSetup<double> vars;
    auto& context = vars.store.curveManager().curveContext("TEST");
    auto& index   = context.index();
    FloatingRateBulletInstrument<double> inst(vars.startDate, vars.endDate, vars.notional, vars.spread, index);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(index.fixingFrequency());
    testStructure<FloatingRateBulletInstrument<double>, double>(inst, schedule, PaymentStructure::BulletOrZero);
    testChangeCurrency<FloatingRateBulletInstrument<double>, double>(inst);
}

TEST(Instrument, FloatingRateBulletInstrumentDual) {
    TestSetup<dual> vars;
    auto& context = vars.store.curveManager().curveContext("TEST");
    auto& index   = context.index();
    FloatingRateBulletInstrument<dual> inst(vars.startDate, vars.endDate, vars.notional, vars.spread, index);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(index.fixingFrequency());
    testStructure<FloatingRateBulletInstrument<dual>, dual>(inst, schedule, PaymentStructure::BulletOrZero);
    testChangeCurrency<FloatingRateBulletInstrument<dual>, dual>(inst);
}