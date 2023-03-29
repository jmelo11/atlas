#include "../pch.hpp"
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/rates/curvecontextstore.hpp>
#include <atlas/visitors/indexer.hpp>

using namespace Atlas;

struct InstrumentVars {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    InterestRate rate          = InterestRate(0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    double spread              = 0.01;
    CurveContextStore& store_  = CurveContextStore::instance();
    InstrumentVars() {
        if (!store_.hasContext("TEST")) {
            FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
            RateIndex index("TEST", Frequency::Annual, Actual360());
            store_.createCurveContext("TEST", curveStrategy, index);
        }
    };
};

TEST(Indexer, FixedRateInstrument) {
    InstrumentVars vars;
    auto& context = vars.store_.at("TEST");
    FixedRateBulletInstrument fixedInst(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate, context);
    size_t dfSize = fixedInst.leg().coupons().size() + fixedInst.leg().redemptions().size();
    Indexer indexer;
    fixedInst.accept(indexer);
    MarketRequest request;
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), 0);
}

TEST(Indexer, FloatingRateInstrument) {
    InstrumentVars vars;
    auto& context = vars.store_.at("TEST");
    FloatingRateBulletInstrument floatInst(vars.startDate, vars.endDate, vars.notional, vars.spread, context, context);
    size_t dfSize  = floatInst.leg().coupons().size() + floatInst.leg().redemptions().size();
    size_t fwdSize = floatInst.leg().coupons().size();
    Indexer indexer;
    MarketRequest request;

    floatInst.accept(indexer);
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}

TEST(Indexer, MultipleInstruments) {
    InstrumentVars vars;
    auto& context = vars.store_.at("TEST");
    FixedRateBulletInstrument fixedInst(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate, context);
    FloatingRateBulletInstrument floatInst(vars.startDate, vars.endDate, vars.notional, vars.spread, context, context);

    size_t dfSize = fixedInst.leg().coupons().size() + fixedInst.leg().redemptions().size();
    dfSize += floatInst.leg().coupons().size() + floatInst.leg().redemptions().size();
    size_t fwdSize = floatInst.leg().coupons().size();

    Indexer indexer;
    MarketRequest request;

    fixedInst.accept(indexer);
    floatInst.accept(indexer);
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}