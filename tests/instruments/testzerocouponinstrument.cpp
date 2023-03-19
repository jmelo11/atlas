#include "../pch.hpp"
#include "general.hpp"
#include <ql/time/daycounters/actual360.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>

TEST(Instrument, ZeroCouponInstrument) {
    FixedInstrumentVars vars;
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(Frequency::Once);
    ZeroCouponInstrument inst(vars.startDate, vars.endDate, vars.notional, vars.rate);
    testStructure<ZeroCouponInstrument>(inst, schedule, PaymentStructure::BulletOrZero);
    testInterest(inst, schedule, vars.rate);
}