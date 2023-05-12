#include "../testsetup.hpp"
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/visitors/cashflowprofiler.hpp>
#include <atlas/visitors/forwardrateforecaster.hpp>

using namespace Atlas;

TEST(CashflowProfiler, FixedRateInstrument) {
    TestSetup<double> vars;
    CashflowProfiler<double> profiler;
    auto& fixedInst = *vars.atlasFixBond;
    profiler.visit(fixedInst);

    const auto& interestProfile   = profiler.interests();
    const auto& redemptionProfile = profiler.redemptions();

    for (const auto& coupon : fixedInst.leg().coupons()) { EXPECT_EQ(interestProfile.at(coupon.paymentDate()), coupon.amount()); }
    for (const auto& redemption : fixedInst.leg().redemptions()) { EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemption.amount()); }
}

TEST(CashflowProfiler, FixedRateInstrumentDual) {
    TestSetup<dual> vars;
    CashflowProfiler<dual> profiler;
    auto& fixedInst = *vars.atlasFixBond;
    profiler.visit(fixedInst);

    const auto& interestProfile   = profiler.interests();
    const auto& redemptionProfile = profiler.redemptions();

    for (const auto& coupon : fixedInst.leg().coupons()) { EXPECT_EQ(interestProfile.at(coupon.paymentDate()), coupon.amount()); }
    for (const auto& redemption : fixedInst.leg().redemptions()) { EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemption.amount()); }
}

TEST(CashflowProfiler, FloatingRateInstrument) {
    TestSetup<double> vars;
    auto& floatInst = *vars.atlasFloatBond;
    CashflowProfiler<double> profiler;
    profiler.visit(floatInst);

    const auto& interestProfile   = profiler.interests();
    const auto& redemptionProfile = profiler.redemptions();

    for (const auto& coupon : floatInst.leg().coupons()) { EXPECT_EQ(interestProfile.at(coupon.paymentDate()), coupon.amount()); }
    for (const auto& redemption : floatInst.leg().redemptions()) { EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemption.amount()); }
}

TEST(CashflowProfiler, FloatingRateInstrumentDual) {
    TestSetup<dual> vars;
    auto& floatInst = *vars.atlasFloatBond;
    CashflowProfiler<dual> profiler;
    profiler.visit(floatInst);

    const auto& interestProfile   = profiler.interests();
    const auto& redemptionProfile = profiler.redemptions();

    for (const auto& coupon : floatInst.leg().coupons()) { EXPECT_EQ(interestProfile.at(coupon.paymentDate()), coupon.amount()); }
    for (const auto& redemption : floatInst.leg().redemptions()) { EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemption.amount()); }
}