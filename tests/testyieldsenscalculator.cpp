#include "pch.hpp"
#include <atlas/visitors/yieldsenscalculator.hpp>

using namespace Atlas;
namespace QL = QuantLib;

TEST(YieldSensCalculator, Deposit) {
    QL::Date startDate(27, QL::Dec, 2022);
    QL::Date endDate(27, QL::Dec, 2023);
    double notional = 1'000'000.0;
    double delta    = 0.0001;
    double rate     = 0.03;

    QL::InterestRate yield(rate, QL::Actual360(), QL::Compounded, QL::Annual);

    Deposit deposit(startDate, endDate, notional, yield);
    YieldSensCalculator sensCalc(yield, startDate);
    sensCalc.visit(deposit);
    double npvBefore = sensCalc.yieldNPV();

    yield = QuantLib::InterestRate(rate + delta, QL::Actual360(), QL::Compounded, QL::Annual);
    sensCalc.yield(yield);
    sensCalc.visit(deposit);

    double sens      = sensCalc.basisPointValue();
    double npvAfter  = sensCalc.yieldNPV();

    EXPECT_FLOAT_EQ(sens, 100);
    EXPECT_FLOAT_EQ(sensCalc.duration(), 100);
    EXPECT_FLOAT_EQ(npvBefore, 1000);
    EXPECT_FLOAT_EQ(npvAfter, 1000);

    EXPECT_NEAR(sens * delta * 10000, npvAfter - npvBefore, delta);
}

TEST(YieldSensCalculator, EqualPaymentProduct) {
    QL::Date startDate(27, QL::Dec, 2022);
    QL::Date endDate(27, QL::Dec, 2025);
    double notional = 1'000'000.0;
    double delta    = 0.0001;
    double rate     = 0.03;

    QL::InterestRate yield(rate, QL::Actual360(), QL::Compounded, QL::Annual);

    EqualPaymentProduct eqPay(startDate, endDate, QuantLib::Frequency::Monthly, notional, yield);
    YieldSensCalculator sensCalc(yield, QuantLib::Date());
    sensCalc.visit(eqPay);
    double npvBefore = sensCalc.yieldNPV();

    yield = QuantLib::InterestRate(rate + delta, QL::Actual360(), QL::Compounded, QL::Annual);
    sensCalc.yield(yield);
    sensCalc.visit(eqPay);

    double sens = sensCalc.basisPointValue();
    double npvAfter = sensCalc.yieldNPV();

    EXPECT_FLOAT_EQ(sens, 100);
    EXPECT_FLOAT_EQ(npvBefore, 1000);
    EXPECT_FLOAT_EQ(npvAfter, 1000);

    EXPECT_NEAR(sens * delta * 10000, npvAfter - npvBefore, delta);
}