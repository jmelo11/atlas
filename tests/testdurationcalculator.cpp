#include "pch.hpp"
#include <atlas/visitors/durationcalculator.hpp>

using namespace Atlas;
namespace QL = QuantLib;

TEST(DurationCalculator, Deposit) {
    QL::Date startDate(27, QL::Dec, 2022);
    QL::Date endDate(27, QL::Dec, 2023);
    double notional = 1'000'000.0;
    double delta    = 0.0001;
    double rate     = 0.03;

    QL::InterestRate yield(rate, QL::Actual360(), QL::Compounded, QL::Annual);
    Deposit deposit(startDate, endDate, notional, yield);

    MarketData marketData;
    deposit.leg().dfIdx(0);
    deposit.leg().coupons().at(0).dfIdx(1);
    deposit.leg().redemptions().at(0).dfIdx(1);

    marketData.dfs.push_back(1);

    double df = yield.discountFactor(startDate, endDate);
    marketData.dfs.push_back(df);

    DurationCalculator cacl(marketData);

    cacl.visit(deposit);
    
    double dur = cacl.results();
    EXPECT_NEAR(dur, 1, 0.001);

}
