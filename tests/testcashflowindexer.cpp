#include "pch.hpp"
#include <atlas/visitors/cashflowindexer.hpp>

using namespace Atlas;

TEST(TestCashflowIndexer, Visitors) {
    QuantLib::Date startDate(1, QuantLib::January, 2018);
    QuantLib::Date endDate(1, QuantLib::January, 2025);
    QuantLib::Frequency frequency   = QuantLib::Frequency::Annual;
    QuantLib::DayCounter dayCounter = QuantLib::Actual360();
    double notional                 = 100;
    QuantLib::InterestRate rate(0.05, dayCounter, QuantLib::Compounding::Simple,
                                QuantLib::Frequency::Annual);

    // deposit
    Deposit prod(startDate, endDate, notional, rate);
    CashflowIndexer indexer;
    prod.accept(indexer);
    MarketRequest request;
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), 3);  // coupon + redemption + start
    EXPECT_EQ(request.fwds.size(), 0);

    auto& tmp = request.dfs.at(0);
    EXPECT_EQ(tmp.date_, startDate);
    EXPECT_EQ(tmp.discountCurve_, "undefined");

    tmp = request.dfs.at(1);
    EXPECT_EQ(tmp.date_, endDate);
    EXPECT_EQ(tmp.discountCurve_, "undefined");

    // equal payment product
    indexer.clear();
    request.dfs.clear();
    request.fwds.clear();

    EqualPaymentProduct prod2(startDate, endDate, frequency, notional, rate);
    prod2.accept(indexer);
    indexer.setRequest(request);
    auto& leg     = prod2.leg();
    auto& coupons = leg.coupons();

    EXPECT_EQ(request.dfs.size(), coupons.size() * 2+1);
    EXPECT_EQ(request.fwds.size(), 0);
    for (size_t i = 0; i < coupons.size(); ++i) {
        auto& coupon = coupons.at(i);
        EXPECT_EQ(coupon.dfIdx(), i+1);
    }
}