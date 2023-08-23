#include "general.hpp"
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>

TEST(Instrument, FloatingRateEqualRedemptionInstrument) {
    TestSetup<double> vars;
    auto& context = vars.store.curveManager().curveContext("TEST");
    auto& index   = context.index();
    FloatingRateEqualRedemptionInstrument<double> inst(vars.startDate, vars.endDate, vars.notional, vars.spread, index);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withTenor(index.tenor());
    testRegularStructure(inst, schedule.dates(), PaymentStructure::EqualRedemptions);
};
