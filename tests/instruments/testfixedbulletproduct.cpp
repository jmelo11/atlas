#include "../pch.hpp"
#include "general.hpp"
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>

TEST(Instrument, FixedRateBulletInstrument) {
    FixedInstrumentVars<double> vars;

    FixedRateBulletInstrument<double> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testInterest<double>(prod, schedule, vars.rate);
    testStructure<FixedRateBulletInstrument<double>,double>(prod, schedule, PaymentStructure::BulletOrZero);
};

TEST(Instrument, FixedRateBulletInstrumentDual) {
    FixedInstrumentVars<dual> vars;

    FixedRateBulletInstrument<dual> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testInterest<dual>(prod, schedule, vars.rate);
    testStructure<FixedRateBulletInstrument<dual>,dual>(prod, schedule, PaymentStructure::BulletOrZero);
};