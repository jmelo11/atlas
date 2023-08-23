#include "general.hpp"
#include <atlas/instruments/fixedrate/zerocouponfixedrateinstrument.hpp>

TEST(Instrument, ZeroCouponInstrument) {
    TestSetup<double> vars;
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(Frequency::Once);
    ZeroCouponFixedRateInstrument<double> inst(vars.startDate, vars.endDate, vars.notional, vars.atlasRate);
    testRegularStructure(inst, schedule.dates(), PaymentStructure::BulletOrZero);
}
