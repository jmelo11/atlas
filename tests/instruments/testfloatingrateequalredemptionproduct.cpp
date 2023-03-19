#include "../pch.hpp"
#include "general.hpp"
#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/rates/rateindex.hpp>

TEST(Instrument, FloatingRateEqualRedemptionInstrument) {
    FloatingInstrumentVars vars;
    FloatingRateEqualRedemptionInstrument inst(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.config);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.config.fixingFrequency());
    testStructure(inst, schedule, PaymentStructure::EqualRedemptions);
};