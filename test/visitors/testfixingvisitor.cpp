
#include "testfixingvisitor.hpp"
#include <atlas/visitors/fixingvisitor.hpp>
#include <atlas/visitors/indexingvisitor.hpp>

TEST(FixingVisitor, NoIndexing) {
    TestFixingVisitor::Common vars;
    FloatingRateBulletInstrument<double> instrument(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.spread, vars.index, vars.discountIdx,
                                                    vars.indexIdx, vars.side);
    MarketData<double> marketData;
    for (size_t i = 0; i < instrument.cashflows().floatingRateCouponCount(); ++i) { marketData.fwds.push_back(0.05); }

    FixingVisitor<double> fixingVisitor(marketData);
    EXPECT_THROW(fixingVisitor(instrument), std::runtime_error);
}

TEST(FixingVisitor, FloatingRateBulletInstrument) {
    TestFixingVisitor::Common vars;
    FloatingRateBulletInstrument<double> instrument(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.spread, vars.index, vars.discountIdx,
                                                    vars.indexIdx, vars.side);
    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);

    MarketData<double> marketData;
    for (size_t i = 0; i < instrument.cashflows().floatingRateCouponCount(); ++i) { marketData.fwds.push_back(0.05); }

    FixingVisitor<double> fixingVisitor(marketData);
    fixingVisitor(instrument);

    for (const auto& cf : instrument.cashflows().floatingRateCoupons()) { EXPECT_TRUE(cf.isFixingSet()); }
}

TEST(FixingVisitor, FloatingRateEqualRedemptionInstrument) {
    TestFixingVisitor::Common vars;
    FloatingRateEqualRedemptionInstrument<double> instrument(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.spread, vars.index, vars.discountIdx,
                                                             vars.indexIdx, vars.side);

    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);

    MarketData<double> marketData;
    for (size_t i = 0; i < instrument.cashflows().floatingRateCouponCount(); ++i) { marketData.fwds.push_back(0.05); }

    FixingVisitor<double> fixingVisitor(marketData);
    fixingVisitor(instrument);

    for (const auto& cf : instrument.cashflows().floatingRateCoupons()) { EXPECT_TRUE(cf.isFixingSet()); }
}

TEST(FixingVisitor, CustomFloatingRateInstrument) {
    TestFixingVisitor::Common vars;
    std::map<Date, double> disbursementMap, redemptionMap;
    disbursementMap[vars.startDate] = vars.notional;
    redemptionMap[vars.endDate]     = vars.notional;

    CustomFloatingRateInstrument<double> instrument(disbursementMap, redemptionMap, vars.spread, vars.index, vars.discountIdx, vars.indexIdx,
                                                    vars.side);
    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);

    MarketData<double> marketData;
    for (size_t i = 0; i < instrument.cashflows().floatingRateCouponCount(); ++i) { marketData.fwds.push_back(0.05); }

    FixingVisitor<double> fixingVisitor(marketData);
    fixingVisitor(instrument);

    for (const auto& cf : instrument.cashflows().floatingRateCoupons()) { EXPECT_TRUE(cf.isFixingSet()); }
}

TEST(FixingVisitor, ZeroCouponFloatingRateInstrument){
    TestFixingVisitor::Common vars;
    ZeroCouponFloatingRateInstrument<double> instrument(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.index, vars.discountIdx,
                                                        vars.indexIdx, vars.side);
    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);

    MarketData<double> marketData;
    for (size_t i = 0; i < instrument.cashflows().floatingRateCouponCount(); ++i) { marketData.fwds.push_back(0.05); }

    FixingVisitor<double> fixingVisitor(marketData);
    fixingVisitor(instrument);

    for (const auto& cf : instrument.cashflows().floatingRateCoupons()) { EXPECT_TRUE(cf.isFixingSet()); }
}