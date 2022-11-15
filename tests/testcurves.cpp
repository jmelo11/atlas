#include "commonvars.hpp"
#include "pch.hpp"
#include <algorithm>
#include <atlas/curves/curve.hpp>
#include <random>

TEST(TestZeroRateCurveNodeConsistency, TestCurves) {
    std::vector<double> times, rates, dfs;
    Date refDate(1, Month::Apr, 2022);
    for (int i = 0; i < 10; i++) {
        times.push_back(i);
        rates.push_back(0.03);
        dfs.push_back(1.0 / (1.0 + 0.03 * i));
    }
    Atlas::ZeroRateCurve<Linear> curve(refDate, times, rates);
    for (int i = 1; i < 10; i++) {
        double fwd = curve.forwardRate(0, times[i]);
        double df  = curve.df(i);
        EXPECT_FLOAT_EQ(fwd, rates[i]);
        EXPECT_FLOAT_EQ(df, dfs[i]);
    }
}

TEST(TestDiscountCurveNodeConsistency, TestCurves) {
    std::vector<double> times, rates, dfs;
    Date refDate(1, Month::Apr, 2022);
    for (int i = 0; i < 10; i++) {
        times.push_back(i);
        rates.push_back(0.03);
        dfs.push_back(1.0 / (1.0 + 0.03 * i));
    }
    Atlas::DiscountCurve<Linear> curve(refDate, times, dfs);
    for (int i = 1; i < 10; i++) {
        EXPECT_FLOAT_EQ(curve.forwardRate(0, times[i]), rates[i]);
        EXPECT_FLOAT_EQ(curve.df(i), dfs[i]);
    }
}

TEST(TestDiscountSorted, TestCurves) {
    std::vector<double> times, rates, dfs, shuffleTimes, shuffleRates;
    std::vector<std::pair<double, double>> nodes;
    Date refDate(1, Month::Apr, 2022);
    for (int i = 0; i < 10; i++) {
        nodes.push_back({i, 0.03});
        times.push_back(i);
        rates.push_back(0.03);
        dfs.push_back(1.0 / (1.0 + 0.03 * i));
    }
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(nodes), std::end(nodes), rng);
    for (size_t i = 0; i < 10; i++) {
        shuffleTimes.push_back(nodes[i].first);
        shuffleRates.push_back(nodes[i].second);
    }
    Atlas::ZeroRateCurve<Linear> curve(refDate, shuffleTimes, shuffleRates);
    for (int i = 1; i < 10; i++) {
        EXPECT_FLOAT_EQ(curve.forwardRate(0, times[i]), rates[i]);
        EXPECT_FLOAT_EQ(curve.df(i), dfs[i]);
    }
}

TEST(TestZeroRateSorted, TestCurves) {
    std::vector<double> times, rates, dfs, shuffleTimes, shuffleDFS;
    std::vector<std::pair<double, double>> nodes;
    Date refDate(1, Month::Apr, 2022);
    for (int i = 0; i < 10; i++) {
        times.push_back(i);
        rates.push_back(0.03);
        dfs.push_back(1.0 / (1.0 + 0.03 * i));
        nodes.push_back({i, 1.0 / (1.0 + 0.03 * i)});
    }
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(nodes), std::end(nodes), rng);
    for (size_t i = 0; i < 10; i++) {
        shuffleTimes.push_back(nodes[i].first);
        shuffleDFS.push_back(nodes[i].second);
    }
    Atlas::DiscountCurve<Linear> curve(refDate, shuffleTimes, shuffleDFS);
    for (int i = 1; i < 10; i++) {
        EXPECT_FLOAT_EQ(curve.forwardRate(0, times[i]), rates[i]);
        EXPECT_FLOAT_EQ(curve.df(i), dfs[i]);
    }
}