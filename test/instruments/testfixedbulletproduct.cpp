#include "general.hpp"
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>

TEST(Instrument, FixedRateBulletInstrument) {
    TestSetup<double> vars;
    FixedRateBulletInstrument<double> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.atlasRate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testRegularStructure(prod, schedule.dates(), PaymentStructure::BulletOrZero);
};

