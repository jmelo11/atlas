#include "pch.hpp"
#include <ql/time/schedule.hpp>
#include <atlas/data/marketdata.hpp>
#include <atlas/visitors/parsolver.hpp>
#include <numeric>

using namespace Atlas;
namespace QL = QuantLib;

TEST(ParSolver, Deposit) {
    QL::Date startDate(1, QL::January, 2018);
    QL::Date endDate(1, QL::January, 2019);
    double notional = 100.0;
    double spread   = 0.0;
    double rate     = 0.05;

    QL::InterestRate interestRate(rate, QL::Actual360(), QL::Compounded, QL::Annual);

    Deposit deposit(startDate, endDate, notional, interestRate);

    MarketData marketData;
    deposit.dfIdx(0);
    deposit.leg().coupons().at(0).dfIdx(1);
    deposit.leg().redemptions().at(0).dfIdx(1);

    marketData.dfs.push_back(1);

    double df = interestRate.discountFactor(startDate, endDate);
    marketData.dfs.push_back(df);

    ParSolver solver(marketData);
    deposit.accept(solver);
    EXPECT_NEAR(solver.results(), rate, 0.0001);

    rate = solver.results();
    deposit.rate(rate);
    NPVCalculator npvCalculator(marketData);
    deposit.accept(npvCalculator);
    EXPECT_NEAR(npvCalculator.results(), notional, 0.0001);
}

TEST(ParSolver, FixedBulletProduct) {
    QL::Date startDate(1, QL::January, 2018);
    QL::Date endDate(1, QL::January, 2019);
    QL::Frequency freq = QL::Frequency::Semiannual;
    double notional    = 100.0;
    double spread      = 0.0;
    double rate        = 0.05;

    QL::InterestRate interestRate(rate, QL::Actual360(), QL::Compounded, QL::Annual);

    FixedRateBulletProduct inst(startDate, endDate, freq, notional, interestRate);
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

    rate = solver.results();
    inst.rate(rate);
    NPVCalculator npvCalculator(marketData);
    inst.accept(npvCalculator);
    EXPECT_NEAR(npvCalculator.results(), notional, 0.0001);
}

TEST(ParSolver, CustomFixedRateProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);

    QuantLib::Frequency freq    = QuantLib::Frequency::Semiannual;
    QuantLib::Schedule schedule = QuantLib::MakeSchedule().from(startDate).to(endDate).withFrequency(freq);

    QuantLib::DayCounter dayCounter = QuantLib::Actual360();

    double rate = 0.03;
    QL::InterestRate interestRate(rate, QL::Actual360(), QL::Compounded, QL::Annual);

    std::vector<double> redemptionAmounts(schedule.dates().size() - 1, 50);  // constant redemptions
    auto notional = std::reduce(redemptionAmounts.begin(), redemptionAmounts.end());
    CustomFixedRateProduct inst(schedule.dates(), redemptionAmounts, interestRate);
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

    rate = solver.results();
    inst.rate(rate);
    NPVCalculator npvCalculator(marketData);
    inst.accept(npvCalculator);
    EXPECT_NEAR(npvCalculator.results(), notional, 0.0001);
}

TEST(ParSolver, EqualPaymentProduct) {
    QL::Date startDate(29, QL::Aug, 2022);
    QL::Date endDate(29, QL::Aug, 2023);
    QL::Frequency freq = QL::Frequency::Semiannual;
    double notional    = 100'000;
    double rate        = 0.03;

    QL::InterestRate interestRate(rate, QL::Actual360(), QL::Simple, QL::Annual);

    EqualPaymentProduct inst(startDate, endDate, freq, notional, interestRate, true);
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
    EXPECT_NEAR(solver.results(), rate, 0.001);

    rate = solver.results();
    inst.rate(rate);
    NPVCalculator npvCalculator(marketData);
    inst.accept(npvCalculator);
    EXPECT_NEAR(npvCalculator.results(), notional, 0.0001);
}

TEST(ParSolver, FloatingBulletProduct) {
    QL::Date startDate(17, QL::Month::November, 2022);
    QL::Date endDate(17, QL::Month::November, 2027);

    double notional = 100'000.0;
    double spread   = 0.0002;

    LIBOR6M index;

    FloatingRateBulletProduct prod(startDate, endDate, notional, spread, index);

    QL::Schedule schedule = QL::MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());

    auto& dates = schedule.dates();
    MarketData marketData;
    double marketRate = 0.03;
    QL::InterestRate rate(marketRate, QL::Actual360(), QL::Simple, QL::Annual);

    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();

    marketData.dfs.push_back(1);
    prod.dfIdx(0);
    for (size_t i = 1; i < dates.size(); ++i) {
        double df = rate.discountFactor(dates.at(0), dates.at(i));
        marketData.dfs.push_back(df);
        marketData.fwds.push_back(marketRate);

        coupons.at(i - 1).dfIdx(marketData.dfs.size() - 1);
        coupons.at(i - 1).fwdIdx(marketData.fwds.size() - 1);
    }
    redemptions.at(0).dfIdx(marketData.dfs.size() - 1);

    ParSolver solver(marketData);
    prod.accept(solver);
    EXPECT_NEAR(solver.results(), -0.0018289, 0.0001);

    spread = solver.results();
    prod.spread(spread);
    NPVCalculator npvCalculator(marketData);
    prod.accept(npvCalculator);
    EXPECT_NEAR(npvCalculator.results(), notional, 0.0001);
}