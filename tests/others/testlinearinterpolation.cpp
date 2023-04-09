#include "../pch.hpp"
#include <atlas/others/interpolations/linearinterpolation.hpp>

using namespace Atlas;

TEST(LinearInterpolationTest, BasicTest) {
    std::vector<double> x_ = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y_ = {0.0, 2.0, 4.0, 6.0};
    LinearInterpolator<double> interpol(x_, y_);
    double result = interpol(1.5);
    EXPECT_DOUBLE_EQ(result, 3.0);
}

TEST(LinearInterpolationTest, ExactValueTest) {
    std::vector<double> x_ = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y_ = {0.0, 2.0, 4.0, 6.0};

    LinearInterpolator<double> interpol(x_, y_);
    double result = interpol(1.0);
    EXPECT_DOUBLE_EQ(result, 2.0);
}

TEST(LinearInterpolationTest, OutOfRangeTest) {
    std::vector<double> x_ = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y_ = {0.0, 2.0, 4.0, 6.0};

    LinearInterpolator<double> interpol(x_, y_);

    EXPECT_THROW(interpol(3.5), std::out_of_range);
}

TEST(LinearInterpolationTest, MismatchedVectorsTest) {
    std::vector<double> x_ = {0.0, 1.0, 2.0};
    std::vector<double> y_ = {0.0, 2.0, 4.0, 6.0};

    EXPECT_THROW(LinearInterpolator<double> interpol(x_, y_), std::invalid_argument);
}