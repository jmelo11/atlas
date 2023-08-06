#include <atlas/others/auxfuncs.hpp>
#include <gtest/gtest.h>

using namespace Atlas;

TEST(CalculateOutstandingTest, BasicTestCases) {
    std::map<Date, double> redemptions   = {{Date(27, Month::Aug, 2023), 50}, {Date(27, Month::Sep, 2023), 50}, {Date(27, Month::Oct, 2023), 50}};
    std::map<Date, double> disbursements = {{Date(27, Month::Jun, 2023), 150}};

    std::vector<std::tuple<Date, Date, double>> result = calculateOutstanding(disbursements, redemptions);

    ASSERT_EQ(3, result.size());
    EXPECT_EQ(Date(27, Month::Jun, 2023), std::get<0>(result[0]));
    EXPECT_EQ(Date(27, Month::Aug, 2023), std::get<1>(result[0]));
    EXPECT_DOUBLE_EQ(150, std::get<2>(result[0]));

    EXPECT_EQ(Date(27, Month::Aug, 2023), std::get<0>(result[1]));
    EXPECT_EQ(Date(27, Month::Sep, 2023), std::get<1>(result[1]));
    EXPECT_DOUBLE_EQ(100, std::get<2>(result[1]));

    EXPECT_EQ(Date(27, Month::Sep, 2023), std::get<0>(result[2]));
    EXPECT_EQ(Date(27, Month::Oct, 2023), std::get<1>(result[2]));
    EXPECT_DOUBLE_EQ(50, std::get<2>(result[2]));
}