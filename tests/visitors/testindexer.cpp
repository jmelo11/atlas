#include "../testsetup.hpp"
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/fundation/marketstore.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/visitors/indexer.hpp>

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

    size_t dfSize  = fixedInst.leg().coupons().size() + 1;  // +2 for the disbursement, equal payment structure
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

    size_t dfSize  = fixedInst.leg().coupons().size() + 1;
    size_t fwdSize = floatInst.leg().coupons().size();

    Indexer<dual> indexer;
    MarketRequest request;

    fixedInst.accept(indexer);
    floatInst.accept(indexer);
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}