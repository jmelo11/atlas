#include "general.hpp"
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>

TEST(Instrument, ZeroCouponInstrument) {
    TestSetup<double> vars;
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(Frequency::Once);
    ZeroCouponInstrument<double> inst(vars.startDate, vars.endDate, vars.notional, vars.atlasRate);
    testStructure<ZeroCouponInstrument<double>, double>(inst, schedule, PaymentStructure::BulletOrZero);
    testInterest<double>(inst, schedule, vars.atlasRate);
    testChangeCurrency<ZeroCouponInstrument<double>, double>(inst);
}

TEST(Instrument, ZeroCouponInstrumentDual) {
    TestSetup<dual> vars;
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(Frequency::Once);
    ZeroCouponInstrument<dual> inst(vars.startDate, vars.endDate, vars.notional, vars.atlasRate);
    testStructure<ZeroCouponInstrument<dual>, dual>(inst, schedule, PaymentStructure::BulletOrZero);
    testInterest<dual>(inst, schedule, vars.atlasRate);
    testChangeCurrency<ZeroCouponInstrument<dual>, dual>(inst);
}