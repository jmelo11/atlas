#include "../pch.hpp"
#include "general.hpp"
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/visitors/cashflowprofiler.hpp>
#include <atlas/visitors/forwardrateforecaster.hpp>

using namespace Atlas;

TEST(CashflowProfiler, FixedRateInstrument) {
    FixedInstrumentVars vars;
    FixedRateBulletInstrument fixedInst(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    CashflowProfiler profiler;
    profiler.visit(fixedInst);

    const auto& interestProfile   = profiler.interests();
    const auto& redemptionProfile = profiler.redemptions();

    for (const auto& coupon : fixedInst.leg().coupons()) { EXPECT_EQ(interestProfile.at(coupon.paymentDate()), coupon.amount()); }
    for (const auto& redemption : fixedInst.leg().redemptions()) { EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemption.amount()); }
}

TEST(CashflowProfiler, FloatingRateInstrument) {
    
}
