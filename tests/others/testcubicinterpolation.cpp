#include "../pch.hpp"
#include <atlas/others/interpolations/cubicinterpolation.hpp>

using namespace Atlas;

TEST(CubicInterpolationTest, BasicTest) {
    std::vector<double> x_ = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y_ = {0.0, 1.0, 8.0, 27.0};

    EXPECT_DOUBLE_EQ(cubicInterpolation<double>(1.0, x_, y_, true), 1.0);
}

TEST(CubicInterpolationTest, ExtrapolationEnabled) {
    std::vector<double> x_ = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y_ = {0.0, 1.0, 8.0, 27.0};

    EXPECT_NO_THROW(cubicInterpolation<double>(-0.5, x_, y_, true));
    EXPECT_NO_THROW(cubicInterpolation<double>(3.5, x_, y_, true));
}

TEST(CubicInterpolationTest, ExtrapolationDisabled) {
    std::vector<double> x_ = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y_ = {0.0, 1.0, 8.0, 27.0};

    EXPECT_THROW(cubicInterpolation<double>(-0.5, x_, y_, false), std::out_of_range);
    EXPECT_THROW(cubicInterpolation<double>(3.5, x_, y_, false), std::out_of_range);
}

TEST(CubicInterpolationTest, UnequalVectorLengths) {
    std::vector<double> x_ = {0.0, 1.0, 2.0};
    std::vector<double> y_ = {0.0, 1.0, 8.0, 27.0};

    EXPECT_THROW(cubicInterpolation<double>(1.5, x_, y_, true), std::invalid_argument);
}