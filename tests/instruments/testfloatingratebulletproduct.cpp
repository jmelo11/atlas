#include "../pch.hpp"
#include "general.hpp"
#include <atlas/basictypes/rateindex.hpp>
#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>

TEST(Instrument, FloatingRateBulletInstrument) {
    FloatingInstrumentVars vars;
    auto index = vars.store_.at("TEST")->index();
    FloatingRateBulletInstrument inst(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store_.at("TEST"));
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(index.fixingFrequency());
    testStructure(inst, schedule, PaymentStructure::BulletOrZero);
}