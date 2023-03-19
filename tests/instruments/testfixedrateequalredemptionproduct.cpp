#include "../pch.hpp"
#include "general.hpp"
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/instruments/fixedrate/fixedrateequalredemptioninstrument.hpp>

TEST(Instrument, FixedRateEqualRedemptionInstrument) {
    FixedInstrumentVars vars;
    FixedRateEqualRedemptionInstrument prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testStructure(prod, schedule, PaymentStructure::EqualRedemptions);
    testInterest(prod, schedule, vars.rate);
};
