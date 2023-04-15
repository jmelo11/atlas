#include "../pch.hpp"
#include "general.hpp"
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/visitors/cashflowprofiler.hpp>
#include <atlas/visitors/forwardrateforecaster.hpp>

using namespace Atlas;

TEST(CashflowProfiler, FixedRateInstrument) {
    FixedInstrumentVars<double> vars;
    FixedRateBulletInstrument<double> fixedInst(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    CashflowProfiler<double> profiler;
    profiler.visit(fixedInst);

    const auto& interestProfile   = profiler.interests();
    const auto& redemptionProfile = profiler.redemptions();

    for (const auto& coupon : fixedInst.leg().coupons()) { EXPECT_EQ(interestProfile.at(coupon.paymentDate()), coupon.amount()); }
    for (const auto& redemption : fixedInst.leg().redemptions()) { EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemption.amount()); }
}

TEST(CashflowProfiler, FixedRateInstrumentDual) {
    FixedInstrumentVars<dual> vars;
    FixedRateBulletInstrument<dual> fixedInst(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    CashflowProfiler<dual> profiler;
    profiler.visit(fixedInst);

    const auto& interestProfile   = profiler.interests();
    const auto& redemptionProfile = profiler.redemptions();

    for (const auto& coupon : fixedInst.leg().coupons()) { EXPECT_EQ(interestProfile.at(coupon.paymentDate()), coupon.amount()); }
    for (const auto& redemption : fixedInst.leg().redemptions()) { EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemption.amount()); }
}

TEST(CashflowProfiler, FloatingRateInstrument) {
    FloatingInstrumentVars<double> vars;
    const auto& context = vars.store_.curveContext("TEST");
    FloatingRateBulletInstrument<double> floatInst(vars.startDate, vars.endDate, vars.notional, vars.spread, context, vars.store_.curveContext("TEST"));
    CashflowProfiler<double> profiler;
    profiler.visit(floatInst);

    const auto& interestProfile   = profiler.interests();
    const auto& redemptionProfile = profiler.redemptions();

    for (const auto& coupon : floatInst.leg().coupons()) { EXPECT_EQ(interestProfile.at(coupon.paymentDate()), coupon.amount()); }
    for (const auto& redemption : floatInst.leg().redemptions()) { EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemption.amount()); }
}


TEST(CashflowProfiler, FloatingRateInstrumentDual) {
    FloatingInstrumentVars<dual> vars;
    auto& context = vars.store_.curveContext("TEST");
    FloatingRateBulletInstrument<dual> floatInst(vars.startDate, vars.endDate, vars.notional, vars.spread, context, vars.store_.curveContext("TEST"));
    CashflowProfiler<dual> profiler;
    profiler.visit(floatInst);

    const auto& interestProfile   = profiler.interests();
    const auto& redemptionProfile = profiler.redemptions();

    for (const auto& coupon : floatInst.leg().coupons()) { EXPECT_EQ(interestProfile.at(coupon.paymentDate()), coupon.amount()); }
    for (const auto& redemption : floatInst.leg().redemptions()) { EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemption.amount()); }
}