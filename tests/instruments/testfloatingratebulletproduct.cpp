#include "../pch.hpp"
#include "general.hpp"
#include <atlas/basictypes/rateindex.hpp>
#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>

TEST(Instrument, FloatingRateBulletInstrument) {
    FloatingInstrumentVars vars;
    FloatingRateBulletInstrument inst(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.config);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.config.fixingFrequency());
    testStructure(inst, schedule, PaymentStructure::BulletOrZero);
}