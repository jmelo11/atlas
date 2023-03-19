#include "../pch.hpp"
#include "general.hpp"
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>

TEST(Instrument, FixedRateBulletInstrument) {
    FixedInstrumentVars vars;
    FixedRateBulletInstrument prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testInterest(prod, schedule, vars.rate);
    testStructure(prod, schedule, PaymentStructure::BulletOrZero);
};