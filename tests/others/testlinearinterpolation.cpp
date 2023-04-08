#include "../pch.hpp"
#include <atlas/others/interpolations/linearinterpolation.hpp>

using namespace Atlas;

TEST(LinearInterpolationTest, BasicTest) {
    std::vector<double> x_ = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y_ = {0.0, 2.0, 4.0, 6.0};

    double result = linearInterpolation(1.5, x_, y_);
    EXPECT_DOUBLE_EQ(result, 3.0);
}

TEST(LinearInterpolationTest, ExactValueTest) {
    std::vector<double> x_ = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y_ = {0.0, 2.0, 4.0, 6.0};

    double result = linearInterpolation(2.0, x_, y_);
    EXPECT_DOUBLE_EQ(result, 4.0);
}

TEST(LinearInterpolationTest, OutOfRangeTest) {
    std::vector<double> x_ = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y_ = {0.0, 2.0, 4.0, 6.0};

    EXPECT_THROW(linearInterpolation(3.5, x_, y_), std::out_of_range);
}

TEST(LinearInterpolationTest, MismatchedVectorsTest) {
    std::vector<double> x_ = {0.0, 1.0, 2.0};
    std::vector<double> y_ = {0.0, 2.0, 4.0, 6.0};

    EXPECT_THROW(linearInterpolation(1.5, x_, y_), std::invalid_argument);
}