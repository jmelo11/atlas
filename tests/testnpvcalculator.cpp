#include "pch.hpp"
#include <atlas/visitors/npvcalculator.hpp>

using namespace Atlas;

TEST(TestNPVCalculator, NPVCalculator) {
    MarketData marketData;
    marketData.dfs  = {0.99, 0.98, 0.97, 0.96, 0.95};
    marketData.fwds = {0.01, 0.02, 0.03, 0.04, 0.05};

    // Deposit
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional = 100;
    QuantLib::InterestRate rate(0.03, QuantLib::Actual360(), QuantLib::Simple, QuantLib::Annual);
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