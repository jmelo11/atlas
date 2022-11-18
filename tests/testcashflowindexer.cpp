#include "pch.hpp"
#include <atlas/visitors/cashflowindexer.hpp>

using namespace Atlas;

TEST(CashflowIndexer, Deposit) {
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
}

TEST(CashflowIndexer, EqualPaymentProduct) {
    QuantLib::Date startDate(1, QuantLib::January, 2018);
    QuantLib::Date endDate(1, QuantLib::January, 2025);
    QuantLib::Frequency frequency   = QuantLib::Frequency::Annual;
    QuantLib::DayCounter dayCounter = QuantLib::Actual360();
    double notional                 = 100;
    QuantLib::InterestRate rate(0.05, dayCounter, QuantLib::Compounding::Simple,
                                QuantLib::Frequency::Annual);

    CashflowIndexer indexer;
    MarketRequest request;

    EqualPaymentProduct prod(startDate, endDate, frequency, notional, rate);
    prod.accept(indexer);
    indexer.setRequest(request);
    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();

    EXPECT_EQ(request.dfs.size(), coupons.size() + redemptions.size() + 1);
    EXPECT_EQ(request.fwds.size(), 0);

    size_t dfCounter = 0;
    EXPECT_EQ(prod.dfIdx(), dfCounter);  // first slot is reserved for start date

    for (size_t i = 0; i < coupons.size() + redemptions.size(); ++i) {
        dfCounter++;
        if (i < coupons.size()) {
            EXPECT_EQ(coupons.at(i).dfIdx(), dfCounter);
        } else {
            EXPECT_EQ(redemptions.at(i - coupons.size()).dfIdx(), dfCounter);
        }
    }
}

TEST(CashflowIndexer, FloatingRateBulletProduct) {
    QuantLib::Date startDate(1, QuantLib::January, 2020);
    QuantLib::Date endDate(1, QuantLib::January, 2025);
    double notional = 100.0;
    double spread   = 0.0;

    LIBOR12M index;
    FloatingRateBulletProduct prod(startDate, endDate, notional, spread, index);

    CashflowIndexer indexer;
    MarketRequest request;

    prod.accept(indexer);
    indexer.setRequest(request);

    auto& leg         = prod.leg();
    auto& coupons     = leg.coupons();
    auto& redemptions = leg.redemptions();

    EXPECT_EQ(request.dfs.size(), coupons.size() + redemptions.size() + 1);
    EXPECT_EQ(request.fwds.size(), coupons.size());

    size_t dfCounter  = 0;
    size_t fwdCounter = 0;
    EXPECT_EQ(prod.dfIdx(), dfCounter);  // first slot is reserved for start date

    for (size_t i = 0; i < coupons.size() + redemptions.size(); ++i) {
        dfCounter++;
        if (i < coupons.size()) {
            EXPECT_EQ(coupons.at(i).dfIdx(), dfCounter);
            EXPECT_EQ(coupons.at(i).fwdIdx(), fwdCounter);
            fwdCounter++;
        } else {
            EXPECT_EQ(redemptions.at(i - coupons.size()).dfIdx(), dfCounter);
        }
    }
}

TEST(CashflowIndexer, MultipleProduct) {
    QuantLib::Date startDate(1, QuantLib::January, 2020);
    QuantLib::Date endDate(1, QuantLib::January, 2025);
    double notional = 100.0;
    double spread   = 0.0;

    LIBOR12M index;
    std::vector<FloatingRateBulletProduct> prods;

    CashflowIndexer indexer;
    MarketRequest request;

    size_t numProds   = 10;
    size_t dfCounter  = 0;
    size_t fwdCounter = 0;

    for (auto& prod : prods) {
        prods.push_back(FloatingRateBulletProduct(startDate, endDate, notional, spread, index));
        indexer.visit(prods.back());
        indexer.setRequest(request);

        auto& leg         = prod.leg();
        auto& coupons     = leg.coupons();
        auto& redemptions = leg.redemptions();

        EXPECT_EQ(request.dfs.size(), coupons.size() + redemptions.size() + 1 + dfCounter);
        EXPECT_EQ(request.fwds.size(), coupons.size() + fwdCounter);
        EXPECT_EQ(prod.dfIdx(), dfCounter);  // first slot is reserved for start date

        for (size_t i = 0; i < coupons.size() + redemptions.size(); ++i) {
            dfCounter++;
            if (i < coupons.size()) {
                EXPECT_EQ(coupons.at(i).dfIdx(), dfCounter);
                EXPECT_EQ(coupons.at(i).fwdIdx(), fwdCounter);
                fwdCounter++;
            } else {
                EXPECT_EQ(redemptions.at(i - coupons.size()).dfIdx(), dfCounter);
            }
        }
        dfCounter++;  // for start date
    }
}