#include "general.hpp"
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>

TEST(Instrument, FixedRateBulletInstrument) {
    TestSetup<double> vars;
    FixedRateBulletInstrument<double> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.atlasRate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testInterest<double>(prod, schedule, vars.atlasRate);
    testStructure<FixedRateBulletInstrument<double>,double>(prod, schedule, PaymentStructure::BulletOrZero);
    testChangeCurrency<FixedRateBulletInstrument<double>,double>(prod);
};

TEST(Instrument, FixedRateBulletInstrumentDual) {
    TestSetup<dual> vars;
    FixedRateBulletInstrument<dual> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.atlasRate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testInterest<dual>(prod, schedule, vars.atlasRate);
    testStructure<FixedRateBulletInstrument<dual>,dual>(prod, schedule, PaymentStructure::BulletOrZero);
    testChangeCurrency<FixedRateBulletInstrument<dual>,dual>(prod);
};