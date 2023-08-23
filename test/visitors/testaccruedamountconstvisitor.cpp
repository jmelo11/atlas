#include "testaccruedamountconstvisitor.hpp"
#include <atlas/visitors/cashflowaggregation/accruedamountconstvisitor.hpp>
#include <atlas/visitors/fixingvisitor.hpp>
#include <atlas/visitors/indexingvisitor.hpp>

using namespace Atlas;

TEST(AccruedAmountConstVisitor, NoFixingSet) {
    TestAccruedAmountConstVisitor::Common vars;
    FloatingRateBulletInstrument<double> instrument(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.index, vars.side);
    AccruedAmountConstVisitor visitor(vars.startDate, vars.endDate);
    EXPECT_THROW(visitor(instrument), std::runtime_error);
}

TEST(AccruedAmountConstVisitor, FixedRateBulletInstrument) {
    TestAccruedAmountConstVisitor::Common vars;
    FixedRateBulletInstrument<double> instrument(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate, vars.discountIdx,
                                                 vars.side);
    AccruedAmountConstVisitor visitor(vars.startDate, vars.endDate);
    visitor(instrument);
    TestAccruedAmountConstVisitor::calculateAccrual(instrument.cashflows(), vars.startDate, vars.endDate);
}

TEST(AccruedAmountConstVisitor, EqualPaymentInstrument) {
    TestAccruedAmountConstVisitor::Common vars;
    EqualPaymentInstrument<double> instrument(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate, vars.discountIdx,
                                              vars.side);
    AccruedAmountConstVisitor visitor(vars.startDate, vars.endDate);
    visitor(instrument);
    TestAccruedAmountConstVisitor::calculateAccrual(instrument.cashflows(), vars.startDate, vars.endDate);
}

TEST(AccruedAmountConstVisitor, ZeroCouponFixedRateInstrument) {
    TestAccruedAmountConstVisitor::Common vars;
    ZeroCouponFixedRateInstrument<double> instrument(vars.startDate, vars.endDate, vars.notional, vars.rate, vars.discountIdx, vars.side);
    AccruedAmountConstVisitor visitor(vars.startDate, vars.endDate);
    visitor(instrument);
    TestAccruedAmountConstVisitor::calculateAccrual(instrument.cashflows(), vars.startDate, vars.endDate);
}

TEST(AccruedAmountConstVisitor, CustomFixedRateInstrument) {
    TestAccruedAmountConstVisitor::Common vars;
    std::map<Date, double> disbursementMap, redemptionMap;
    disbursementMap[vars.startDate] = vars.notional;
    redemptionMap[vars.endDate]     = vars.notional;

    CustomFixedRateInstrument<double> instrument(disbursementMap, redemptionMap, vars.rate, vars.discountIdx, vars.side);
    AccruedAmountConstVisitor visitor(vars.startDate, vars.endDate);
    visitor(instrument);
    TestAccruedAmountConstVisitor::calculateAccrual(instrument.cashflows(), vars.startDate, vars.endDate);
}

TEST(AccruedAmountConstVisitor, FloatingRateBulletInstrument) {
    TestAccruedAmountConstVisitor::Common vars;
    FloatingRateBulletInstrument<double> instrument(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.index, vars.discountIdx,
                                                    vars.indexIdx, vars.side);

    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);

    MarketData<double> marketData;
    for (size_t i = 0; i < instrument.cashflows().floatingRateCouponCount(); ++i) { marketData.fwds.push_back(0.05); }

    FixingVisitor<double> fixingVisitor(marketData);
    fixingVisitor(instrument);

    AccruedAmountConstVisitor visitor(vars.startDate, vars.endDate);
    visitor(instrument);
    TestAccruedAmountConstVisitor::calculateAccrual(instrument.cashflows(), vars.startDate, vars.endDate);
}

TEST(AccruedAmountConstVisitor, FloatingRateEqualRedemptionInstrument) {
    TestAccruedAmountConstVisitor::Common vars;
    FloatingRateEqualRedemptionInstrument<double> instrument(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.index, vars.discountIdx,
                                                             vars.indexIdx, vars.side);

    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);

    MarketData<double> marketData;
    for (size_t i = 0; i < instrument.cashflows().floatingRateCouponCount(); ++i) { marketData.fwds.push_back(0.05); }

    FixingVisitor<double> fixingVisitor(marketData);
    fixingVisitor(instrument);

    AccruedAmountConstVisitor visitor(vars.startDate, vars.endDate);
    visitor(instrument);
    TestAccruedAmountConstVisitor::calculateAccrual(instrument.cashflows(), vars.startDate, vars.endDate);
}

TEST(AccruedAmountConstVisitor, CustomFloatingRateInstrument) {
    TestAccruedAmountConstVisitor::Common vars;
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

    AccruedAmountConstVisitor visitor(vars.startDate, vars.endDate);
    visitor(instrument);
    TestAccruedAmountConstVisitor::calculateAccrual(instrument.cashflows(), vars.startDate, vars.endDate);
}

TEST(AccruedAmountConstVisitor, ZeroCouponFloatingRateInstrument) {
    TestAccruedAmountConstVisitor::Common vars;
    ZeroCouponFloatingRateInstrument<double> instrument(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.index, vars.discountIdx,
                                                        vars.indexIdx, vars.side);

    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);

    MarketData<double> marketData;
    for (size_t i = 0; i < instrument.cashflows().floatingRateCouponCount(); ++i) { marketData.fwds.push_back(0.05); }

    FixingVisitor<double> fixingVisitor(marketData);
    fixingVisitor(instrument);

    AccruedAmountConstVisitor visitor(vars.startDate, vars.endDate);
    visitor(instrument);
    TestAccruedAmountConstVisitor::calculateAccrual(instrument.cashflows(), vars.startDate, vars.endDate);
}
