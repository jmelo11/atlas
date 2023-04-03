#include "../pch.hpp"
#include "general.hpp"
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>

TEST(Instrument, ZeroCouponInstrument) {
    FixedInstrumentVars<double> vars;
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(Frequency::Once);
    ZeroCouponInstrument<double> inst(vars.startDate, vars.endDate, vars.notional, vars.rate);
    testStructure<ZeroCouponInstrument<double>, double>(inst, schedule, PaymentStructure::BulletOrZero);
    testInterest<double>(inst, schedule, vars.rate);
}

TEST(Instrument, ZeroCouponInstrumentDual) {
    FixedInstrumentVars<dual> vars;
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(Frequency::Once);
    ZeroCouponInstrument<dual> inst(vars.startDate, vars.endDate, vars.notional, vars.rate);
    testStructure<ZeroCouponInstrument<dual>, dual>(inst, schedule, PaymentStructure::BulletOrZero);
    testInterest<dual>(inst, schedule, vars.rate);
}