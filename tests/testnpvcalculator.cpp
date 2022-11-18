#include "pch.hpp"
#include <ql/time/daycounters/thirty360.hpp>
#include <ql/time/schedule.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <atlas/visitors/parsolver.hpp>

using namespace Atlas;
namespace QL = QuantLib;

TEST(NPVCalculator, Deposit) {
    MarketData marketData;
    marketData.dfs  = {0.99, 0.98, 0.97, 0.96, 0.95};
    marketData.fwds = {0.01, 0.02, 0.03, 0.04, 0.05};

    // Deposit
    QL::Date startDate(1, QL::Month::Aug, 2020);
    QL::Date endDate(1, QL::Month::Aug, 2021);
    double notional = 100;
    QL::InterestRate rate(0.03, QL::Actual360(), QL::Simple, QL::Annual);
    Deposit prod(startDate, endDate, notional, rate);

    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();
    double amount     = coupons.at(0).amount() + redemptions.at(0).amount();

    NPVCalculator visitor(marketData);
    for (size_t i = 0; i < marketData.dfs.size(); ++i) {
        coupons.at(0).dfIdx(i);
        redemptions.at(0).dfIdx(i);

        prod.accept(visitor);
        double npv = visitor.results();
        EXPECT_FLOAT_EQ(npv, amount * marketData.dfs.at(i));
        visitor.clear();
    }
}

TEST(NPVCalculator, FloatingRateBulletProduct) {
    QL::Date startDate(17, QL::Month::November, 2022);
    QL::Date endDate(17, QL::Month::November, 2027);

    double notional = 100'000.0;
    double spread   = 0.0;

    LIBOR6M index;

    FloatingRateBulletProduct prod(startDate, endDate, notional, spread, index);

    QL::Schedule schedule =
        QL::MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());

    auto& dates = schedule.dates();
    MarketData marketData;
    double marketRate = 0.03;
    QL::InterestRate rate(marketRate, QL::Actual360(), QL::Simple, QL::Annual);

    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();

    for (size_t i = 1; i < dates.size(); ++i) {
        double df = rate.discountFactor(dates.at(0), dates.at(i));
        marketData.dfs.push_back(df);
        marketData.fwds.push_back(marketRate);
        coupons.at(i - 1).dfIdx(marketData.dfs.size() - 1);
        coupons.at(i - 1).fwdIdx(marketData.fwds.size() - 1);
    }
    redemptions.at(0).dfIdx(marketData.dfs.size() - 1);

    NPVCalculator visitor(marketData);
    prod.accept(visitor);
    double npv = visitor.results();

    double nonSensNPV = visitor.nonSensNPV();
    double sensNPV    = visitor.sensNPV();

    EXPECT_NEAR(npv, 100'857.41597, 0.0001);
    EXPECT_FLOAT_EQ(nonSensNPV + sensNPV, npv);
}
