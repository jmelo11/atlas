#include "../pch.hpp"
#include "general.hpp"
#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/rates/rateindex.hpp>

TEST(Instrument, FloatingRateEqualRedemptionInstrument) {
    FloatingInstrumentVars vars;
    auto index = vars.store_.at("TEST")->index();
    FloatingRateEqualRedemptionInstrument inst(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store_.at("TEST"));
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(index.fixingFrequency());
    testStructure(inst, schedule, PaymentStructure::EqualRedemptions);
};