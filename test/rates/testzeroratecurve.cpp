
#include <ql/math/interpolations/linearinterpolation.hpp>
#include <ql/termstructures/yield/zerocurve.hpp>
#include <atlas/others/interpolations/linearinterpolation.hpp>
#include <atlas/rates/yieldtermstructure/zeroratecurve.hpp>
#include <gtest/gtest.h>

using namespace Atlas;

TEST(ZeroRateCurve, DiscountFactors) {
    std::vector<Date> dates;
    std::vector<double> rates;
    for (int i = 0; i < 10; ++i) {
        dates.push_back(Date(1, Month::Jan, 2020) + i * TimeUnit::Years);
        rates.push_back(0.05);
    }

    Date refDate                                    = Date(1, Month::Jan, 2020);
    QuantLib::Settings::instance().evaluationDate() = refDate;
    DayCounter dayCounter                           = Actual360();
    Compounding compounding                         = Compounding::Continuous;
    Frequency frequency                             = Frequency::Annual;
    Calendar calendar                               = QuantLib::NullCalendar();
    QuantLib::InterpolatedZeroCurve<QuantLib::Linear> qlCurve(dates, rates, dayCounter, calendar, QuantLib::Linear());
    ZeroRateStrategy<double, LinearInterpolator<double>> curve(dates, rates, dayCounter, compounding, frequency);

    for (auto date : dates) {
        double expected = qlCurve.discount(date);
        double actual   = curve.discount(date);
        EXPECT_NEAR(actual, expected, 1e-9);
    }
}

TEST(ZeroRateCurve, ForwardRates) {
    std::vector<Date> dates;
    std::vector<double> rates;
    for (int i = 0; i < 10; ++i) {
        dates.push_back(Date(1, Month::Jan, 2020) + i * TimeUnit::Years);
        rates.push_back(0.05);
    }

    Date refDate                                    = Date(1, Month::Jan, 2020);
    QuantLib::Settings::instance().evaluationDate() = refDate;
    DayCounter dayCounter                           = Actual360();
    Compounding compounding                         = Compounding::Continuous;
    Frequency frequency                             = Frequency::Annual;
    Calendar calendar                               = QuantLib::NullCalendar();
    QuantLib::InterpolatedZeroCurve<QuantLib::Linear> qlCurve(dates, rates, dayCounter, calendar, QuantLib::Linear());
    ZeroRateStrategy<double, LinearInterpolator<double>> curve(dates, rates, dayCounter, compounding, frequency);

    for (size_t i = 0; i < dates.size() - 1; ++i) {
        Date start      = dates[i];
        Date end        = dates[i + 1];
        double expected = qlCurve.forwardRate(start, end, dayCounter, compounding, frequency);
        double actual   = curve.forwardRate(start, end, dayCounter, compounding, frequency);
        EXPECT_NEAR(actual, expected, 1e-9);
    }
}

// new interface

TEST(ZeroRateTermStructure, DiscountFactors) {
    std::vector<Date> dates;
    std::vector<double> rates;
    for (int i = 0; i < 10; ++i) {
        dates.push_back(Date(1, Month::Jan, 2020) + i * TimeUnit::Years);
        rates.push_back(0.05);
    }

    Date refDate                                    = Date(1, Month::Jan, 2020);
    QuantLib::Settings::instance().evaluationDate() = refDate;
    DayCounter dayCounter                           = Actual360();
    Compounding compounding                         = Compounding::Continuous;
    Frequency frequency                             = Frequency::Annual;
    Calendar calendar                               = QuantLib::NullCalendar();
    QuantLib::InterpolatedZeroCurve<QuantLib::Linear> qlCurve(dates, rates, dayCounter, calendar, QuantLib::Linear());
    BaseYieldTermStructure curve = ZeroRateTermStructure<LinearInterpolator>(dates, rates, dayCounter, compounding, frequency);

    for (auto date : dates) {
        double expected = qlCurve.discount(date);
        double actual   = curve.discount(date);
        EXPECT_NEAR(actual, expected, 1e-9);
    }
}

TEST(ZeroRateTermStructure, ForwardRates) {
    std::vector<Date> dates;
    std::vector<double> rates;
    for (int i = 0; i < 10; ++i) {
        dates.push_back(Date(1, Month::Jan, 2020) + i * TimeUnit::Years);
        rates.push_back(0.05);
    }

    Date refDate                                    = Date(1, Month::Jan, 2020);
    QuantLib::Settings::instance().evaluationDate() = refDate;
    DayCounter dayCounter                           = Actual360();
    Compounding compounding                         = Compounding::Continuous;
    Frequency frequency                             = Frequency::Annual;
    Calendar calendar                               = QuantLib::NullCalendar();
    QuantLib::InterpolatedZeroCurve<QuantLib::Linear> qlCurve(dates, rates, dayCounter, calendar, QuantLib::Linear());
    BaseYieldTermStructure curve = ZeroRateTermStructure<LinearInterpolator>(dates, rates, dayCounter, compounding, frequency);

    for (size_t i = 0; i < dates.size() - 1; ++i) {
        Date start      = dates[i];
        Date end        = dates[i + 1];
        double expected = qlCurve.forwardRate(start, end, dayCounter, compounding, frequency);
        double actual   = curve.forwardRate(start, end, dayCounter, compounding, frequency);
        EXPECT_NEAR(actual, expected, 1e-9);
    }
}