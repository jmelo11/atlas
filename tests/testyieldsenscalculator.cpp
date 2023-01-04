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
    
}

TEST(YieldSensCalculator, EqualPaymentProduct) {
    QL::Date startDate(27, QL::Dec, 2022);
    QL::Date endDate(27, QL::Dec, 2025);
    double notional = 1'000'000.0;
    double delta    = 0.0001;
    double rate     = 0.03;

    QL::InterestRate yield(rate, QL::Actual360(), QL::Compounded, QL::Annual);
    EqualPaymentProduct eqPay(startDate, endDate, QuantLib::Frequency::Monthly, notional, yield);
    
}