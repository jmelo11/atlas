#include "../testsetup.hpp"
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>

using namespace Atlas;

TEST(FixedRateCouponTest, Attributes) {
    TestSetup<double> vars;
    FixedRateCoupon<double> coupon(vars.startDate, vars.endDate, vars.notional, vars.atlasRate, vars.store.curveContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());
}

TEST(FixedRateCouponTest, ValidDates) {
    // Test case with valid dates
    Date couponStartDate(1, Month::January, 2023);
    Date couponEndDate(30, Month::June, 2023);
    double notional = 1000;
    InterestRate<double> rate(0.05, Actual360());  // Assuming double as the template type
    Date refStartDate(1, Month::January, 2023);
    Date refEndDate(31, Month::March, 2023);

    FixedRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, rate);
    double expectedAccruedAmount = notional * (rate.compoundFactor(refStartDate, refEndDate) - 1.0);

    EXPECT_DOUBLE_EQ(coupon.accruedAmount(refStartDate, refEndDate), expectedAccruedAmount);
}

TEST(FixedRateCouponTest, DatesBeforeReference) {
    // Test case with coupon dates before the reference dates
    Date couponStartDate(1, Month::December, 2022);
    Date couponEndDate(28, Month::February, 2023);
    double notional = 5000;
    InterestRate<double> rate(0.05, Actual360());  // Assuming double as the template type
    Date refStartDate(1, Month::January, 2024);
    Date refEndDate(31, Month::March, 2024);

    FixedRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, rate);
    double expectedAccruedAmount = 0.0;  // Coupon dates are before the reference dates

    EXPECT_DOUBLE_EQ(coupon.accruedAmount(refStartDate, refEndDate), expectedAccruedAmount);
}

TEST(FixedRateCouponTest, DatesAfterReference) {
    // Test case with coupon dates after the reference dates
    Date couponStartDate(1, Month::April, 2023);
    Date couponEndDate(30, Month::June, 2023);
    double notional = 2000;
    InterestRate<double> rate(0.05, Actual360());  // Assuming double as the template type
    Date refStartDate(1, Month::January, 2023);
    Date refEndDate(31, Month::March, 2023);

    FixedRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, rate);
    double expectedAccruedAmount = 0.0;  // Coupon dates are after the reference dates

    EXPECT_DOUBLE_EQ(coupon.accruedAmount(refStartDate, refEndDate), expectedAccruedAmount);
}

TEST(FixedRateCouponTest, FirstOutSecondInDates) {
    // Test case where first date is before the coupon start and second date is before or equal to the coupon end
    Date couponStartDate(1, Month::April, 2023);
    Date couponEndDate(30, Month::June, 2023);
    double notional = 2000;
    InterestRate<double> rate(0.05, Actual360());  // Assuming double as the template type
    Date refStartDate(1, Month::January, 2023);
    Date refEndDate(31, Month::May, 2023);

    FixedRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, rate);

    // Expected accrued amount is the same as if the first date was the coupon start date
    double expectedAccruedAmount = notional * (rate.compoundFactor(couponStartDate, refEndDate) - 1.0);

    EXPECT_DOUBLE_EQ(coupon.accruedAmount(refStartDate, refEndDate), expectedAccruedAmount);
}

TEST(FixedRateCouponTest, FirstInSecondOutDates) {
    // Test case where first date is before or equal to the coupon start and second date is after the coupon end
    Date couponStartDate(1, Month::January, 2023);
    Date couponEndDate(30, Month::June, 2023);
    double notional = 2000;
    InterestRate<double> rate(0.05, Actual360());  // Assuming double as the template type
    Date refStartDate(1, Month::April, 2023);
    Date refEndDate(31, Month::July, 2023);

    FixedRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, rate);

    // Expected accrued amount is the same as if the second date was the coupon end date
    double expectedAccruedAmount = notional * (rate.compoundFactor(refStartDate, couponEndDate) - 1.0);

    EXPECT_DOUBLE_EQ(coupon.accruedAmount(refStartDate, refEndDate), expectedAccruedAmount);
}

TEST(FixedRateCouponTest, SameStartAndEndDate) {
    // Test case with same start and end dates
    Date couponStartDate(1, Month::January, 2023);
    Date couponEndDate(1, Month::January, 2023);
    double notional = 1500;
    InterestRate<double> rate(0.05, Actual360());  // Assuming double as the template type
    Date refStartDate(1, Month::January, 2023);
    Date refEndDate(31, Month::March, 2023);

    FixedRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, rate);
    double expectedAccruedAmount = 0.0;  // Start date is the same as the end date

    EXPECT_DOUBLE_EQ(coupon.accruedAmount(refStartDate, refEndDate), expectedAccruedAmount);
}

TEST(FixedRateCouponTest, AccrualSameDates) {
    // Test case with different day count conventions
    double notional = 2500;
    InterestRate<double> rate(0.05, Actual360());  // Assuming double as the template type
    Date couponStartDate(1, Month::February, 2023);
    Date couponEndDate(31, Month::May, 2023);

    FixedRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, rate);
    double expectedAccruedAmount = notional * (rate.compoundFactor(couponStartDate, couponEndDate) - 1.0);

    EXPECT_DOUBLE_EQ(coupon.accruedAmount(couponStartDate, couponEndDate), expectedAccruedAmount);
    EXPECT_DOUBLE_EQ(expectedAccruedAmount, coupon.amount());
}

TEST(FloatingRateCouponTest, Attributes) {
    TestSetup<double> vars;
    FloatingRateCoupon<double> coupon(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.store.rateIndexContext("TEST"));

    EXPECT_EQ(vars.startDate, coupon.startDate());
    EXPECT_EQ(vars.endDate, coupon.endDate());
    EXPECT_EQ(vars.endDate, coupon.paymentDate());
    EXPECT_NEAR(val(vars.spread), val(coupon.spread()), 1e-6);
}

TEST(FloatingRateCouponTest, ValidDates) {
    // Test case with valid dates
    Date couponStartDate(1, Month::January, 2023);
    Date couponEndDate(30, Month::June, 2023);

    TestSetup<double> vars;
    double notional = 1000;
    double spread   = 0.0;
    auto& context   = vars.store.rateIndexContext("TEST");
    auto& index     = context.object();
    InterestRate<double> rate(0.05, index.dayCounter(), index.rateCompounding(), index.rateFrequency());  // Assuming double as the template type
    Date refStartDate(1, Month::January, 2023);
    Date refEndDate(31, Month::March, 2023);

    FloatingRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, spread, vars.store.rateIndexContext("TEST"));
    coupon.fixing(rate.rate());
    double expectedAccruedAmount = notional * (rate.compoundFactor(refStartDate, refEndDate) - 1.0);
    EXPECT_DOUBLE_EQ(coupon.accruedAmount(refStartDate, refEndDate), expectedAccruedAmount);
}

TEST(FloatingRateCouponTest, DatesBeforeReference) {
    // Test case with coupon dates before the reference dates
    Date couponStartDate(1, Month::December, 2022);
    Date couponEndDate(28, Month::February, 2023);
    double notional = 5000;
    Date refStartDate(1, Month::January, 2024);
    Date refEndDate(31, Month::March, 2024);

    TestSetup<double> vars;
    double spread = 0.0;
    auto& context = vars.store.rateIndexContext("TEST");
    auto& index   = context.object();
    InterestRate<double> rate(0.05, index.dayCounter(), index.rateCompounding(), index.rateFrequency());  // Assuming double as the template type
    FloatingRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, spread, vars.store.rateIndexContext("TEST"));

    double expectedAccruedAmount = 0.0;  // Coupon dates are before the reference dates

    EXPECT_DOUBLE_EQ(coupon.accruedAmount(refStartDate, refEndDate), expectedAccruedAmount);
}

TEST(FloatingRateCouponTest, DatesAfterReference) {
    // Test case with coupon dates after the reference dates
    Date couponStartDate(1, Month::April, 2023);
    Date couponEndDate(30, Month::June, 2023);

    Date refStartDate(1, Month::January, 2023);
    Date refEndDate(31, Month::March, 2023);

    TestSetup<double> vars;
    double spread   = 0.0;
    auto& context   = vars.store.rateIndexContext("TEST");
    auto& index     = context.object();
    double notional = 5000;
    InterestRate<double> rate(0.05, index.dayCounter(), index.rateCompounding(), index.rateFrequency());  // Assuming double as the template type
    FloatingRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, spread, vars.store.rateIndexContext("TEST"));
    double expectedAccruedAmount = 0.0;  // Coupon dates are after the reference dates

    EXPECT_DOUBLE_EQ(coupon.accruedAmount(refStartDate, refEndDate), expectedAccruedAmount);
}

TEST(FloatingRateCouponTest, FirstOutSecondInDates) {
    // Test case where first date is before the coupon start and second date is before or equal to the coupon end
    Date couponStartDate(1, Month::April, 2023);
    Date couponEndDate(30, Month::June, 2023);

    Date refStartDate(1, Month::January, 2023);
    Date refEndDate(31, Month::May, 2023);

    TestSetup<double> vars;
    double notional = 5000;
    double spread   = 0.0;
    auto& context   = vars.store.rateIndexContext("TEST");
    auto& index     = context.object();
    InterestRate<double> rate(0.05, index.dayCounter(), index.rateCompounding(), index.rateFrequency());  // Assuming double as the template type
    FloatingRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, spread, vars.store.rateIndexContext("TEST"));
    coupon.fixing(rate.rate());
    // Expected accrued amount is the same as if the first date was the coupon start date
    double expectedAccruedAmount = notional * (rate.compoundFactor(couponStartDate, refEndDate) - 1.0);

    EXPECT_DOUBLE_EQ(coupon.accruedAmount(refStartDate, refEndDate), expectedAccruedAmount);
}

TEST(FloatingRateCouponTest, FirstInSecondOutDates) {
    // Test case where first date is before or equal to the coupon start and second date is after the coupon end
    Date couponStartDate(1, Month::January, 2023);
    Date couponEndDate(30, Month::June, 2023);
    Date refStartDate(1, Month::April, 2023);
    Date refEndDate(31, Month::July, 2023);

    TestSetup<double> vars;
    double notional = 5000;
    double spread   = 0.0;
    auto& context   = vars.store.rateIndexContext("TEST");
    auto& index     = context.object();
    InterestRate<double> rate(0.05, index.dayCounter(), index.rateCompounding(), index.rateFrequency());  // Assuming double as the template type
    FloatingRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, spread, vars.store.rateIndexContext("TEST"));
    coupon.fixing(rate.rate());
    // Expected accrued amount is the same as if the second date was the coupon end date
    double expectedAccruedAmount = notional * (rate.compoundFactor(refStartDate, couponEndDate) - 1.0);

    EXPECT_DOUBLE_EQ(coupon.accruedAmount(refStartDate, refEndDate), expectedAccruedAmount);
}

TEST(FloatingRateCouponTest, SameStartAndEndDate) {
    // Test case with same start and end dates
    Date couponStartDate(1, Month::January, 2023);
    Date couponEndDate(1, Month::January, 2023);

    Date refStartDate(1, Month::January, 2023);
    Date refEndDate(31, Month::March, 2023);

    TestSetup<double> vars;
    double notional = 5000;
    double spread   = 0.0;
    auto& context   = vars.store.rateIndexContext("TEST");
    auto& index     = context.object();
    InterestRate<double> rate(0.05, index.dayCounter(), index.rateCompounding(), index.rateFrequency());  // Assuming double as the template type
    FloatingRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, spread, vars.store.rateIndexContext("TEST"));

    double expectedAccruedAmount = 0.0;  // Start date is the same as the end date

    EXPECT_DOUBLE_EQ(coupon.accruedAmount(refStartDate, refEndDate), expectedAccruedAmount);
}

TEST(FloatingRateCouponTest, AccrualSameDates) {
    // Test case with different day count conventions

    Date couponStartDate(1, Month::February, 2023);
    Date couponEndDate(31, Month::May, 2023);

    TestSetup<double> vars;
    double notional = 5000;
    double spread   = 0.0;
    auto& context   = vars.store.rateIndexContext("TEST");
    auto& index     = context.object();
    InterestRate<double> rate(0.05, index.dayCounter(), index.rateCompounding(), index.rateFrequency());  // Assuming double as the template type
    FloatingRateCoupon<double> coupon(couponStartDate, couponEndDate, notional, spread, vars.store.rateIndexContext("TEST"));
    coupon.fixing(rate.rate());
    double expectedAccruedAmount = notional * (rate.compoundFactor(couponStartDate, couponEndDate) - 1.0);

    EXPECT_DOUBLE_EQ(expectedAccruedAmount, coupon.accruedAmount(couponStartDate, couponEndDate));
    EXPECT_DOUBLE_EQ(expectedAccruedAmount, coupon.amount());
}