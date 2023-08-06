#include "general.hpp"
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>

TEST(Instrument, FloatingRateBulletInstrument) {
    TestSetup<double> vars;
    auto& context = vars.store.curveManager().curveContext("TEST");
    auto& index   = context.index();
    FloatingRateBulletInstrument<double> inst(vars.startDate, vars.endDate, vars.notional, vars.spread, index);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(index.fixingFrequency());
    testRegularStructure(inst, schedule.dates(), PaymentStructure::BulletOrZero);
}
