#include "testindexingvisitor.hpp"
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/visitors/indexingvisitor.hpp>

// New Indexing Visitor
TEST(IndexingVisitor, NoDiscountContextIdx) {
    TestIndexingVisitor::Common vars;
    FixedRateBulletInstrument<double> instrument(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate, vars.side);
    IndexingVisitor indexingVisitor;
    EXPECT_THROW(indexingVisitor(instrument), std::runtime_error);
}

TEST(IndexingVisitor, NoIndexContextIdx) {
    TestIndexingVisitor::Common vars;
    FloatingRateBulletInstrument<double> instrument(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.index, vars.side);
    IndexingVisitor indexingVisitor;
    EXPECT_THROW(indexingVisitor(instrument), std::runtime_error);
}

TEST(IndexingVisitor, FixedRateBulletInstrument) {
    TestIndexingVisitor::Common vars;
    FixedRateBulletInstrument<double> instrument(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate, vars.discountIdx,
                                                 vars.side);
    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);
    TestIndexingVisitor::testindexer(instrument.cashflows(), indexingVisitor.getResults());
}

TEST(IndexingVisitor, EqualPaymentInstrument) {
    TestIndexingVisitor::Common vars;
    EqualPaymentInstrument<double> instrument(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate, vars.discountIdx,
                                              vars.side);
    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);
    TestIndexingVisitor::testindexer(instrument.cashflows(), indexingVisitor.getResults());
}

TEST(IndexingVisitor, ZeroCouponInstrument) {
    TestIndexingVisitor::Common vars;
    ZeroCouponInstrument<double> instrument(vars.startDate, vars.endDate, vars.notional, vars.rate, vars.discountIdx, vars.side);
    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);
    TestIndexingVisitor::testindexer(instrument.cashflows(), indexingVisitor.getResults());
}

TEST(IndexingVisitor, CustomFixedRateInstrument) {
    TestIndexingVisitor::Common vars;
    std::map<Date, double> disbursementMap, redemptionMap;
    disbursementMap[vars.startDate] = vars.notional;
    redemptionMap[vars.endDate]     = vars.notional;

    CustomFixedRateInstrument<double> instrument(disbursementMap, redemptionMap, vars.rate, vars.discountIdx, vars.side);
    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);
    TestIndexingVisitor::testindexer(instrument.cashflows(), indexingVisitor.getResults());
}

TEST(IndexingVisitor, FloatingRateBulletInstrument) {
    TestIndexingVisitor::Common vars;
    FloatingRateBulletInstrument<double> instrument(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.index, vars.discountIdx,
                                                    vars.indexIdx, vars.side);
    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);
    TestIndexingVisitor::testindexer(instrument.cashflows(), indexingVisitor.getResults());
}

TEST(IndexingVisitor, FloatingRateEqualRedemptionInstrument) {
    TestIndexingVisitor::Common vars;
    FloatingRateEqualRedemptionInstrument<double> instrument(vars.startDate, vars.endDate, vars.notional, vars.spread, vars.index, vars.discountIdx,
                                                             vars.indexIdx, vars.side);
    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);
    TestIndexingVisitor::testindexer(instrument.cashflows(), indexingVisitor.getResults());
}

TEST(IndexingVisitor, CustomFloatingRateInstrument) {
    TestIndexingVisitor::Common vars;
    std::map<Date, double> disbursementMap, redemptionMap;
    disbursementMap[vars.startDate] = vars.notional;
    redemptionMap[vars.endDate]     = vars.notional;

    CustomFloatingRateInstrument<double> instrument(disbursementMap, redemptionMap, vars.spread, vars.index, vars.discountIdx, vars.indexIdx,
                                                    vars.side);
    IndexingVisitor indexingVisitor;
    indexingVisitor(instrument);
    TestIndexingVisitor::testindexer(instrument.cashflows(), indexingVisitor.getResults());
}