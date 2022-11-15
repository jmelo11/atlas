#include "pch.hpp"
#include <atlas/data/marketdata.hpp>
#include <atlas/visitors/parsolver.hpp>

using namespace Atlas;

TEST(TestDepositRateSolver, RateSolver) {
    QuantLib::Date startDate(1, QuantLib::January, 2018);
    QuantLib::Date endDate(1, QuantLib::January, 2019);
    double notional = 100.0;
    double spread   = 0.0;
    double rate     = 0.05;

    QuantLib::InterestRate interestRate(rate, QuantLib::Actual360(), QuantLib::Compounded,
                                        QuantLib::Annual);

    Deposit deposit(startDate, endDate, notional, interestRate);

    MarketData marketData;
    deposit.dfIdx(0);
    deposit.leg().coupons().at(0).dfIdx(1);
    deposit.leg().redemptions().at(0).dfIdx(1);

    marketData.dfs.push_back(1);

    double df = 1 / interestRate.compoundFactor(startDate, endDate);
    marketData.dfs.push_back(df);

    ParSolver solver(marketData);
    deposit.accept(solver);
    EXPECT_NEAR(solver.results(), rate, 0.0001);
}

TEST(TestBulletRateSolver, RateSolver) {
    QuantLib::Date startDate(1, QuantLib::January, 2018);
    QuantLib::Date endDate(1, QuantLib::January, 2019);
    QuantLib::Frequency freq = QuantLib::Frequency::Semiannual;
    double notional          = 100.0;
    double spread            = 0.0;
    double rate              = 0.05;

    QuantLib::InterestRate interestRate(rate, QuantLib::Actual360(), QuantLib::Compounded,
                                        QuantLib::Annual);

    FixedBulletProduct inst(startDate, endDate, freq, notional, interestRate);
    auto& leg         = inst.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();

    MarketData marketData;

    inst.dfIdx(0);
    marketData.dfs.push_back(1);

    for (auto& coupon : coupons) {
        double df = 1 / interestRate.compoundFactor(startDate, coupon.paymentDate());
        marketData.dfs.push_back(df);
        coupon.dfIdx(marketData.dfs.size() - 1);
    }

    for (auto& redemption : redemptions) {
        double df = 1 / interestRate.compoundFactor(startDate, redemption.paymentDate());
        marketData.dfs.push_back(df);
        redemption.dfIdx(marketData.dfs.size() - 1);
    }

    ParSolver solver(marketData);
    inst.accept(solver);
    EXPECT_NEAR(solver.results(), rate, 0.0001);
}