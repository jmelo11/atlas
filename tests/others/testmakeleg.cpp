#include "../pch.hpp"
#include "../testsetup.hpp"
#include <atlas/cashflows/legs/makeleg.hpp>

using namespace Atlas;

// Test building a fixed rate leg
TEST(MakeLegTest, FixedRateLegTest) {
    TestSetup<double> testSetup;

    MakeLeg<double, FixedRateLeg<double>> makeLeg;
    makeLeg.paymentFrequency(Frequency::Semiannual)
        .startDate(testSetup.startDate)
        .endDate(testSetup.endDate)
        .currency(USD())
        .discountCurveContext(&testSetup.store.curveContext("TEST"))
        .paymentConvention(BusinessDayConvention::ModifiedFollowing)
        .notional(testSetup.notional)
        .calendar(UnitedStates(UnitedStates::GovernmentBond));

    InterestRate<double> rate(0.02, Actual360());
    makeLeg.rate(rate);

    FixedRateLeg<double> leg = makeLeg.build();
    EXPECT_EQ(leg.coupons().size(), 2);
    EXPECT_EQ(leg.redemptions().size(), 1);
    EXPECT_EQ(leg.coupons()[0].rate().rate(), 0.02);
}

// Test building a floating rate leg
TEST(MakeLegTest, FloatingRateLegTest) {
    TestSetup<double> testSetup;
    Schedule schedule = MakeSchedule()
                            .from(testSetup.startDate)
                            .to(testSetup.endDate)
                            .withFrequency(Frequency::Quarterly)
                            .withCalendar(UnitedStates(UnitedStates::GovernmentBond))
                            .withConvention(BusinessDayConvention::ModifiedFollowing);
    size_t numCoupons = schedule.dates().size() - 1;

    MakeLeg<double, FloatingRateLeg<double>> makeLeg;
    std::vector<double> redemptions = std::vector<double>(numCoupons, testSetup.notional / numCoupons);
    makeLeg.notional(testSetup.notional)
        .paymentFrequency(Frequency::Quarterly)
        .startDate(testSetup.startDate)
        .endDate(testSetup.endDate)
        .currency(USD())
        .discountCurveContext(&testSetup.store.curveContext("TEST"))
        .paymentConvention(BusinessDayConvention::ModifiedFollowing)
        .redemptions(redemptions)
        .calendar(UnitedStates(UnitedStates::GovernmentBond));

    makeLeg.spread(0.01);
    makeLeg.rateIndexContext(&testSetup.store.rateIndexContext("TEST"));

    FloatingRateLeg<double> leg = makeLeg.build();

    EXPECT_EQ(leg.coupons().size(), schedule.dates().size() - 1);
    EXPECT_EQ(leg.redemptions().size(), schedule.dates().size() - 1);
}

// Test throwing an error for mismatched redemption and schedule sizes
TEST(MakeLegTest, RedemptionSizeErrorTest) {
    TestSetup<double> testSetup;
    MakeLeg<double, FloatingRateLeg<double>> makeLeg;

    std::vector<double> redemptions = {1.0, 1.0, 1.0, 1.0, 1.0};
    makeLeg.notional(100.0)
        .paymentFrequency(Frequency::Quarterly)
        .startDate(testSetup.startDate)
        .endDate(testSetup.endDate)
        .currency(USD())
        .discountCurveContext(&testSetup.store.curveContext("TEST"))
        .paymentConvention(BusinessDayConvention::ModifiedFollowing)
        .redemptions(redemptions)
        .calendar(UnitedStates(UnitedStates::GovernmentBond));

    makeLeg.spread(0.01);
    makeLeg.rateIndexContext(&testSetup.store.rateIndexContext("TEST"));

    EXPECT_THROW(makeLeg.build(), std::runtime_error);
}
