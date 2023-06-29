#include "../testsetup.hpp"
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/fundation/marketstore.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/newvisitors/indexingvisitor.hpp>
using namespace Atlas;

TEST(Indexer, FixedRateInstrument) {
    TestSetup<double> vars;
    auto& fixedInst = *vars.atlasFixBond;

    size_t dfSize = fixedInst.leg().coupons().size() + 1;  // +1 for the disbursement
    Indexer<double> indexer;
    fixedInst.accept(indexer);
    MarketRequest request;
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), 0);
}

TEST(Indexer, FloatingRateInstrument) {
    TestSetup<double> vars;
    auto& floatInst = *vars.atlasFloatBond;
    size_t dfSize   = floatInst.leg().coupons().size() + 1;  // +1 for the disbursement
    size_t fwdSize  = floatInst.leg().coupons().size();
    Indexer<double> indexer;
    MarketRequest request;

    floatInst.accept(indexer);
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}

TEST(Indexer, MultipleInstruments) {
    TestSetup<double> vars;
    auto& fixedInst = *vars.atlasFixBond;
    auto& floatInst = *vars.atlasFloatBond;

    std::vector<Date> paymentDates;
    for (auto& coupon : fixedInst.leg().coupons()) { paymentDates.push_back(coupon.paymentDate()); }
    for (auto& coupon : floatInst.leg().coupons()) {
        if (std::find(paymentDates.begin(), paymentDates.end(), coupon.paymentDate()) == paymentDates.end()) {
            paymentDates.push_back(coupon.paymentDate());
        }
    }
    size_t dfSize  = paymentDates.size() + 1;
    size_t fwdSize = floatInst.leg().coupons().size();

    Indexer<double> indexer;
    MarketRequest request;

    fixedInst.accept(indexer);
    floatInst.accept(indexer);
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}

TEST(Indexer, FixedRateInstrumentDual) {
    TestSetup<dual> vars;
    auto& fixedInst = *vars.atlasFixBond;
    size_t dfSize   = fixedInst.leg().coupons().size() + 1;
    Indexer<dual> indexer;
    fixedInst.accept(indexer);
    MarketRequest request;
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), 0);
}

TEST(Indexer, FloatingRateInstrumentDual) {
    TestSetup<dual> vars;
    auto& floatInst = *vars.atlasFloatBond;
    size_t dfSize   = floatInst.leg().coupons().size() + 1;
    size_t fwdSize  = floatInst.leg().coupons().size();
    Indexer<dual> indexer;
    MarketRequest request;

    floatInst.accept(indexer);
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}

TEST(Indexer, MultipleInstrumentsDual) {
    TestSetup<dual> vars;
    auto& fixedInst = *vars.atlasFixBond;
    auto& floatInst = *vars.atlasFloatBond;

    std::vector<Date> paymentDates;
    for (auto& coupon : fixedInst.leg().coupons()) { paymentDates.push_back(coupon.paymentDate()); }
    for (auto& coupon : floatInst.leg().coupons()) {
        if (std::find(paymentDates.begin(), paymentDates.end(), coupon.paymentDate()) == paymentDates.end()) {
            paymentDates.push_back(coupon.paymentDate());
        }
    }
    size_t dfSize  = paymentDates.size() + 1;
    size_t fwdSize = floatInst.leg().coupons().size();

    Indexer<dual> indexer;
    MarketRequest request;

    fixedInst.accept(indexer);
    floatInst.accept(indexer);
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}

// New Indexing Visitor

TEST(IndexingVisitor, FixedRateInstrument) {
    TestSetup<double> vars;
    auto& fixedInst = *vars.atlasFixBond;

    size_t dfSize = fixedInst.leg().coupons().size() + 1;  // +1 for the disbursement
    IndexingVisitor<double> indexer;
    indexer(fixedInst);
    MarketRequest request = indexer.getResults();

    for (const auto& coupon : fixedInst.leg().coupons()) {
        EXPECT_TRUE(coupon.isIndexed());
        EXPECT_NE(coupon.dfIdx(), SIZE_T_MAX);
    }
    for (const auto& redemption : fixedInst.leg().redemptions()) {
        EXPECT_TRUE(redemption.isIndexed());
        EXPECT_NE(redemption.dfIdx(), SIZE_T_MAX);
    }
    EXPECT_TRUE(fixedInst.disbursement().isIndexed());
    EXPECT_NE(fixedInst.disbursement().dfIdx(), SIZE_T_MAX);

    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), 0);
}

TEST(IndexingVisitor, FloatingRateInstrument) {
    TestSetup<double> vars;
    auto& floatInst = *vars.atlasFloatBond;
    size_t dfSize   = floatInst.leg().coupons().size() + 1;  // +1 for the disbursement
    size_t fwdSize  = floatInst.leg().coupons().size();

    IndexingVisitor<double> indexer;
    indexer(floatInst);
    MarketRequest request = indexer.getResults();

    for (const auto& coupon : floatInst.leg().coupons()) {
        EXPECT_TRUE(coupon.isIndexed());
        EXPECT_NE(coupon.dfIdx(), SIZE_T_MAX);
    }
    for (const auto& redemption : floatInst.leg().redemptions()) {
        EXPECT_TRUE(redemption.isIndexed());
        EXPECT_NE(redemption.dfIdx(), SIZE_T_MAX);
    }
    EXPECT_TRUE(floatInst.disbursement().isIndexed());
    EXPECT_NE(floatInst.disbursement().dfIdx(), SIZE_T_MAX);

    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}

TEST(IndexingVisitor, MultipleInstruments) {
    TestSetup<double> vars;
    auto& fixedInst = *vars.atlasFixBond;
    auto& floatInst = *vars.atlasFloatBond;

    std::vector<Date> paymentDates;
    for (auto& coupon : fixedInst.leg().coupons()) { paymentDates.push_back(coupon.paymentDate()); }
    for (auto& coupon : floatInst.leg().coupons()) {
        if (std::find(paymentDates.begin(), paymentDates.end(), coupon.paymentDate()) == paymentDates.end()) {
            paymentDates.push_back(coupon.paymentDate());
        }
    }
    size_t dfSize  = paymentDates.size() + 1;
    size_t fwdSize = floatInst.leg().coupons().size();

    IndexingVisitor<double> indexer;
    indexer(fixedInst);
    indexer(floatInst);
    MarketRequest request = indexer.getResults();

    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}

TEST(IndexingVisitor, FixedRateInstrumentDual) {
    TestSetup<dual> vars;
    auto& fixedInst = *vars.atlasFixBond;
    size_t dfSize   = fixedInst.leg().coupons().size() + 1;

    IndexingVisitor<dual> indexer;
    indexer(fixedInst);
    MarketRequest request = indexer.getResults();

    for (const auto& coupon : fixedInst.leg().coupons()) {
        EXPECT_TRUE(coupon.isIndexed());
        EXPECT_NE(coupon.dfIdx(), SIZE_T_MAX);
    }
    for (const auto& redemption : fixedInst.leg().redemptions()) {
        EXPECT_TRUE(redemption.isIndexed());
        EXPECT_NE(redemption.dfIdx(), SIZE_T_MAX);
    }
    EXPECT_TRUE(fixedInst.disbursement().isIndexed());
    EXPECT_NE(fixedInst.disbursement().dfIdx(), SIZE_T_MAX);

    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), 0);
}

TEST(IndexingVisitor, FloatingRateInstrumentDual) {
    TestSetup<dual> vars;
    auto& floatInst = *vars.atlasFloatBond;
    size_t dfSize   = floatInst.leg().coupons().size() + 1;
    size_t fwdSize  = floatInst.leg().coupons().size();

    IndexingVisitor<dual> indexer;
    indexer(floatInst);
    MarketRequest request = indexer.getResults();

    for (const auto& coupon : floatInst.leg().coupons()) {
        EXPECT_TRUE(coupon.isIndexed());
        EXPECT_NE(coupon.dfIdx(), SIZE_T_MAX);
    }
    for (const auto& redemption : floatInst.leg().redemptions()) {
        EXPECT_TRUE(redemption.isIndexed());
        EXPECT_NE(redemption.dfIdx(), SIZE_T_MAX);
    }
    EXPECT_TRUE(floatInst.disbursement().isIndexed());
    EXPECT_NE(floatInst.disbursement().dfIdx(), SIZE_T_MAX);

    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}

TEST(IndexingVisitor, MultipleInstrumentsDual) {
    TestSetup<dual> vars;
    auto& fixedInst = *vars.atlasFixBond;
    auto& floatInst = *vars.atlasFloatBond;

    std::vector<Date> paymentDates;
    for (auto& coupon : fixedInst.leg().coupons()) { paymentDates.push_back(coupon.paymentDate()); }
    for (auto& coupon : floatInst.leg().coupons()) {
        if (std::find(paymentDates.begin(), paymentDates.end(), coupon.paymentDate()) == paymentDates.end()) {
            paymentDates.push_back(coupon.paymentDate());
        }
    }
    size_t dfSize  = paymentDates.size() + 1;
    size_t fwdSize = floatInst.leg().coupons().size();

    IndexingVisitor<dual> indexer;
    indexer(fixedInst);
    indexer(floatInst);
    MarketRequest request = indexer.getResults();

    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}