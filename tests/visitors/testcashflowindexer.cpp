#include "../pch.hpp"
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/visitors/cashflowindexer.hpp>

using namespace Atlas;

struct InstrumentVars {
    Date startDate                = Date(1, Month::Aug, 2020);
    Date endDate                  = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency    = Frequency::Semiannual;
    double notional               = 100;
    InterestRate rate             = InterestRate(0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    double spread                 = 0.01;
    RateIndexConfiguration config = RateIndexConfiguration("test", Frequency::Semiannual);
};

TEST(CashflowIndexer, FixedRateInstrument) {
    InstrumentVars vars;
    FixedRateBulletInstrument fixedInst(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    size_t dfSize = fixedInst.leg().coupons().size() + fixedInst.leg().redemptions().size();
    CashflowIndexer indexer;
    fixedInst.accept(indexer);
    MarketRequest request;
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), 0);
}

TEST(CashflowIndexer, FloatingRateInstrument) {
    InstrumentVars vars;
    FloatingRateBulletInstrument floatInst(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.config);
    size_t dfSize  = floatInst.leg().coupons().size() + floatInst.leg().redemptions().size();
    size_t fwdSize = floatInst.leg().coupons().size();
    CashflowIndexer indexer;
    MarketRequest request;

    floatInst.accept(indexer);
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}

TEST(CashflowIndexer, MultipleInstruments) {
    InstrumentVars vars;
    FixedRateBulletInstrument fixedInst(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    FloatingRateBulletInstrument floatInst(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.config);

    size_t dfSize = fixedInst.leg().coupons().size() + fixedInst.leg().redemptions().size();
    dfSize += floatInst.leg().coupons().size() + floatInst.leg().redemptions().size();
    size_t fwdSize = floatInst.leg().coupons().size();

    CashflowIndexer indexer;
    MarketRequest request;

    fixedInst.accept(indexer);
    floatInst.accept(indexer);
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}