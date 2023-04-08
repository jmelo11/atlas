#include "../pch.hpp"
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/fundation/marketstore.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/visitors/indexer.hpp>

using namespace Atlas;

template <typename adouble>
struct InstrumentVars {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    adouble rateValue          = 0.03;
    InterestRate<adouble> rate = InterestRate(rateValue, Actual360(), Compounding::Simple, Frequency::Annual);
    adouble spread             = 0.01;
    MarketStore store_         = MarketStore();
    InstrumentVars() {
        if (!store_.hasCurveContext("TEST")) {
            FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
            RateIndex index("TEST", Frequency::Annual, Actual360());
            store_.createCurveContext("TEST", curveStrategy, index);
        }
    };
};

TEST(Indexer, FixedRateInstrument) {
    InstrumentVars<double> vars;
    auto& context = vars.store_.curveContext("TEST");
    FixedRateBulletInstrument<double> fixedInst(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate, context);
    size_t dfSize = fixedInst.leg().coupons().size() + fixedInst.leg().redemptions().size() + 1;
    Indexer<double> indexer;
    fixedInst.accept(indexer);
    MarketRequest request;
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), 0);
}

TEST(Indexer, FloatingRateInstrument) {
    InstrumentVars<double> vars;
    auto& context = vars.store_.curveContext("TEST");
    FloatingRateBulletInstrument floatInst(vars.startDate, vars.endDate, vars.notional, vars.spread, context, context);
    size_t dfSize  = floatInst.leg().coupons().size() + floatInst.leg().redemptions().size() + 1;
    size_t fwdSize = floatInst.leg().coupons().size();
    Indexer<double> indexer;
    MarketRequest request;

    floatInst.accept(indexer);
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}

TEST(Indexer, MultipleInstruments) {
    InstrumentVars<double> vars;
    auto& context = vars.store_.curveContext("TEST");
    FixedRateBulletInstrument<double> fixedInst(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate, context);
    FloatingRateBulletInstrument<double> floatInst(vars.startDate, vars.endDate, vars.notional, vars.spread, context, context);

    size_t dfSize = fixedInst.leg().coupons().size() + fixedInst.leg().redemptions().size() + 2;  // +2 for the disbursement
    dfSize += floatInst.leg().coupons().size() + floatInst.leg().redemptions().size();
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
    InstrumentVars<dual> vars;
    auto& context = vars.store_.curveContext("TEST");
    FixedRateBulletInstrument<dual> fixedInst(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate, context);
    size_t dfSize = fixedInst.leg().coupons().size() + fixedInst.leg().redemptions().size() + 1;
    Indexer<dual> indexer;
    fixedInst.accept(indexer);
    MarketRequest request;
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), 0);
}

TEST(Indexer, FloatingRateInstrumentDual) {
    InstrumentVars<dual> vars;
    auto& context = vars.store_.curveContext("TEST");
    FloatingRateBulletInstrument<dual> floatInst(vars.startDate, vars.endDate, vars.notional, vars.spread, context, context);
    size_t dfSize  = floatInst.leg().coupons().size() + floatInst.leg().redemptions().size() + 1;
    size_t fwdSize = floatInst.leg().coupons().size();
    Indexer<dual> indexer;
    MarketRequest request;

    floatInst.accept(indexer);
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}

TEST(Indexer, MultipleInstrumentsDual) {
    InstrumentVars<dual> vars;
    auto& context = vars.store_.curveContext("TEST");
    FixedRateBulletInstrument<dual> fixedInst(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate, context);
    FloatingRateBulletInstrument<dual> floatInst(vars.startDate, vars.endDate, vars.notional, vars.spread, context, context);

    size_t dfSize = fixedInst.leg().coupons().size() + fixedInst.leg().redemptions().size() + 2;
    dfSize += floatInst.leg().coupons().size() + floatInst.leg().redemptions().size();
    size_t fwdSize = floatInst.leg().coupons().size();

    Indexer<dual> indexer;
    MarketRequest request;

    fixedInst.accept(indexer);
    floatInst.accept(indexer);
    indexer.setRequest(request);
    EXPECT_EQ(request.dfs.size(), dfSize);
    EXPECT_EQ(request.fwds.size(), fwdSize);
}