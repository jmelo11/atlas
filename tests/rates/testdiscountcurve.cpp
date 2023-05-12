#include <gtest/gtest.h>
#include <ql/termstructures/yield/discountcurve.hpp>
#include <atlas/others/interpolations/linearinterpolation.hpp>
#include <atlas/others/interpolations/loglinearinterpolation.hpp>
#include <atlas/rates/yieldtermstructure/discountcurve.hpp>

using namespace Atlas;

TEST(DiscountCurve, BasicTest) {
    std::vector<Date> dates       = {Date(1, Month::Jan, 2020), Date(1, Month::Jan, 2021), Date(1, Month::Jan, 2022)};
    std::vector<double> discounts = {1, 0.9, 0.85};
    DayCounter dayCounter         = Actual360();
    DiscountStrategy<double, LinearInterpolator<double>> curve(dates, discounts, dayCounter);

    QuantLib::Settings::instance().evaluationDate() = dates[0];
    QuantLib::DiscountCurve qlCurve(dates, discounts, dayCounter);

    Date date       = Date(1, Month::Jan, 2021);
    double expected = qlCurve.discount(date);
    double actual   = curve.discount(date);

    EXPECT_NEAR(actual, expected, 1e-9);
}

TEST(DiscountCurve, DiscountFactorTest) {
    std::vector<Date> dates       = {Date(1, Month::Jan, 2020), Date(1, Month::Jan, 2021), Date(1, Month::Jan, 2022)};
    std::vector<double> discounts = {1, 0.9, 0.85};
    DayCounter dayCounter         = Actual360();
    DiscountStrategy<double, LogLinearInterpolator<double>> curve(dates, discounts, dayCounter);

    QuantLib::Settings::instance().evaluationDate() = dates[0];
    QuantLib::DiscountCurve qlCurve(dates, discounts, dayCounter);

    double time     = 1.0;  // Time corresponding to the second date (1 year)
    double expected = qlCurve.discount(time);
    double actual   = curve.discount(time);

    EXPECT_NEAR(actual, expected, 1e-9);
}

TEST(DiscountCurve, InvalidArgumentsTest) {
    std::vector<Date> dates       = {Date(1, Month::Jan, 2020), Date(1, Month::Jan, 2021)};
    std::vector<double> discounts = {1, 0.9};
    DayCounter dayCounter         = Actual360();

    auto f = [&]() { DiscountStrategy<double, LinearInterpolator<double>> curve(std::vector<Date>{}, discounts, dayCounter); };
    EXPECT_THROW(f(), std::invalid_argument);

    auto g = [&]() { DiscountStrategy<double, LinearInterpolator<double>> curve(dates, std::vector<double>{}, dayCounter); };
    EXPECT_THROW(g(), std::invalid_argument);

    auto h = [&]() { DiscountStrategy<double, LinearInterpolator<double>> curve(dates, std::vector<double>{1}, dayCounter); };
    EXPECT_THROW(h(), std::invalid_argument);
}

TEST(DiscountCurve, ForwardRateTest) {
    std::vector<Date> dates       = {Date(1, Month::Jan, 2020), Date(1, Month::Jan, 2024), Date(1, Month::Jan, 2027)};
    std::vector<double> discounts = {1, 0.9, 0.85};
    DayCounter dayCounter         = Actual360();
    DiscountStrategy<double, LogLinearInterpolator<double>> curve(dates, discounts, dayCounter);
    curve.enableExtrapolation(true);
    QuantLib::Settings::instance().evaluationDate() = dates[0];
    QuantLib::DiscountCurve qlCurve(dates, discounts, dayCounter);

    Date start_date  = Date(1, Month::Jan, 2020);
    Date end_date    = Date(1, Month::Jan, 2021);
    Compounding comp = Compounding::Continuous;
    Frequency freq   = Frequency::Annual;

    QuantLib::InterestRate expected_rate = qlCurve.forwardRate(start_date, end_date, dayCounter, comp, freq);
    double actual_rate                   = curve.forwardRate(start_date, end_date, dayCounter, comp, freq);

    EXPECT_NEAR(actual_rate, expected_rate.rate(), 1e-9);
}