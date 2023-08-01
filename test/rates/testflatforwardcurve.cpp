
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <gtest/gtest.h>

using namespace Atlas;

TEST(FlatForwardCurve, DiscountFactors) {
    double rate  = 0.05;
    Date refDate = Date(1, Month::Jan, 2020);

    QuantLib::Settings::instance().evaluationDate() = refDate;
    DayCounter dayCounter                           = Actual360();
    Compounding compounding                         = Compounding::Simple;
    Frequency frequency                             = Frequency::Annual;
    Calendar calendar                               = QuantLib::NullCalendar();
    QuantLib::FlatForward qlCurve(refDate, rate, dayCounter, compounding, frequency);
    FlatForwardTermStructure<double> curve(refDate, rate, dayCounter, compounding, frequency);

    Date startDate = refDate;
    Date endDate   = startDate + 5 * TimeUnit::Years;
    Frequency freq = Frequency::Monthly;

    Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(freq).withConvention(BusinessDayConvention::Unadjusted);

    for (auto date : schedule.dates()) {
        double expected = qlCurve.discount(date);
        double actual   = curve.discount(date);
        EXPECT_NEAR(actual, expected, 1e-9);
    }
}

TEST(FlatForwardCurve, ForwardRates) {
    double rate  = 0.05;
    Date refDate = Date(1, Month::Jan, 2020);

    QuantLib::Settings::instance().evaluationDate() = refDate;
    DayCounter dayCounter                           = Actual360();
    Compounding compounding                         = Compounding::Simple;
    Frequency frequency                             = Frequency::Annual;
    Calendar calendar                               = QuantLib::NullCalendar();
    QuantLib::FlatForward qlCurve(refDate, rate, dayCounter, compounding, frequency);
    FlatForwardTermStructure<double> curve(refDate, rate, dayCounter, compounding, frequency);

    Date startDate = refDate;
    Date endDate   = startDate + 5 * TimeUnit::Years;
    Frequency freq = Frequency::Monthly;

    Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(freq).withConvention(BusinessDayConvention::Unadjusted);

    for (size_t i = 0; i < schedule.size() - 1; ++i) {
        Date start      = schedule[i];
        Date end        = schedule[i + 1];
        double expected = qlCurve.forwardRate(start, end, dayCounter, compounding, frequency);
        double actual   = curve.forwardRate(start, end, dayCounter, compounding, frequency);
        EXPECT_NEAR(actual, expected, 1e-9);
    }
}

// new interface

TEST(FlatForwardTermStructure, DiscountFactors) {
    double rate  = 0.05;
    Date refDate = Date(1, Month::Jan, 2020);

    QuantLib::Settings::instance().evaluationDate() = refDate;
    DayCounter dayCounter                           = Actual360();
    Compounding compounding                         = Compounding::Simple;
    Frequency frequency                             = Frequency::Annual;
    Calendar calendar                               = QuantLib::NullCalendar();
    QuantLib::FlatForward qlCurve(refDate, rate, dayCounter, compounding, frequency);
    YieldTermStructure curve = FlatForwardTermStructure(refDate, rate, dayCounter, compounding, frequency);

    Date startDate = refDate;
    Date endDate   = startDate + 5 * TimeUnit::Years;
    Frequency freq = Frequency::Monthly;

    Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(freq).withConvention(BusinessDayConvention::Unadjusted);

    for (auto date : schedule.dates()) {
        double expected = qlCurve.discount(date);
        double actual   = curve.discount(date);
        EXPECT_NEAR(actual, expected, 1e-9);
    }
}

TEST(FlatForwardTermStructure, ForwardRates) {
    double rate  = 0.05;
    Date refDate = Date(1, Month::Jan, 2020);

    QuantLib::Settings::instance().evaluationDate() = refDate;
    DayCounter dayCounter                           = Actual360();
    Compounding compounding                         = Compounding::Simple;
    Frequency frequency                             = Frequency::Annual;
    Calendar calendar                               = QuantLib::NullCalendar();
    QuantLib::FlatForward qlCurve(refDate, rate, dayCounter, compounding, frequency);
    YieldTermStructure curve = FlatForwardTermStructure(refDate, rate, dayCounter, compounding, frequency);

    Date startDate = refDate;
    Date endDate   = startDate + 5 * TimeUnit::Years;
    Frequency freq = Frequency::Monthly;

    Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(freq).withConvention(BusinessDayConvention::Unadjusted);

    for (size_t i = 0; i < schedule.size() - 1; ++i) {
        Date start      = schedule[i];
        Date end        = schedule[i + 1];
        double expected = qlCurve.forwardRate(start, end, dayCounter, compounding, frequency);
        double actual   = curve.forwardRate(start, end, dayCounter, compounding, frequency);
        EXPECT_NEAR(actual, expected, 1e-9);
    }
}