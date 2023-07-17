#include "general.hpp"
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>

TEST(Instrument, EqualPaymentInstrument) {
    TestSetup<double> vars;
    EqualPaymentInstrument<double> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.atlasRate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testInterest<double>(prod, schedule, vars.atlasRate);

    const auto& leg         = prod.leg();
    const auto& coupons     = leg.coupons();
    const auto& redemptions = leg.redemptions();

    // test if payments are equal
    std::vector<Cashflow<double>> cashflows;
    cashflows.insert(cashflows.end(), coupons.begin(), coupons.end());
    cashflows.insert(cashflows.end(), redemptions.begin(), redemptions.end());

    std::map<Date, double> payments;
    for (const auto& cashflow : cashflows) {
        if (payments.find(cashflow.paymentDate()) == payments.end()) { payments[cashflow.paymentDate()] = 0; }
        payments[cashflow.paymentDate()] += cashflow.amount();
    }

    double firstPayment = payments.begin()->second;
    for (const auto& payment : payments) {
        EXPECT_DOUBLE_EQ(firstPayment, payment.second);
        firstPayment = payment.second;
    }

    testChangeCurrency<EqualPaymentInstrument<double>, double>(prod);
}

TEST(Instrument, EqualPaymentInstrumentChangeSide) {
    TestSetup<double> vars;
    EqualPaymentInstrument<double> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.atlasRate);
    EqualPaymentInstrument<double> prod2(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.atlasRate, Side::Short);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testInterest<double>(prod, schedule, vars.atlasRate);

    const auto& nCoupons = prod.leg().coupons().size();
    for (size_t i = 0; i < nCoupons; ++i) {
        EXPECT_DOUBLE_EQ(prod.leg().coupon(i).amount(), -prod2.leg().coupon(i).amount());
        EXPECT_DOUBLE_EQ(prod.leg().redemption(i).amount(), -prod2.leg().redemption(i).amount());
    }
    EXPECT_DOUBLE_EQ(prod.disbursement().amount(), -prod2.disbursement().amount());
}

TEST(Instrument, EqualPaymentInstrumentChangeRate) {
    TestSetup<double> vars;
    EqualPaymentInstrument<double> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.atlasRate, Side::Long, true);

    const auto& leg         = prod.leg();
    const auto& coupons     = leg.coupons();
    const auto& redemptions = leg.redemptions();

    // test if payments are equal
    std::map<Date, double> payments;
    for (const auto& cashflow : coupons) {
        if (payments.find(cashflow.paymentDate()) == payments.end()) { payments[cashflow.paymentDate()] = 0; }
        payments[cashflow.paymentDate()] += cashflow.amount();
    }
    for (const auto& cashflow : redemptions) {
        if (payments.find(cashflow.paymentDate()) == payments.end()) { payments[cashflow.paymentDate()] = 0; }
        payments[cashflow.paymentDate()] += cashflow.amount();
    }

    InterestRate<double> rate(0.06, Actual360());
    prod.rate(rate);
    std::map<Date, double> payments2;
    for (const auto& cashflow : coupons) {
        if (payments2.find(cashflow.paymentDate()) == payments2.end()) { payments2[cashflow.paymentDate()] = 0; }
        payments2[cashflow.paymentDate()] += cashflow.amount();
    }
    for (const auto& cashflow : redemptions) {
        if (payments2.find(cashflow.paymentDate()) == payments2.end()) { payments2[cashflow.paymentDate()] = 0; }
        payments2[cashflow.paymentDate()] += cashflow.amount();
    }

    double amount2 = payments2[leg.coupon(0).paymentDate()];
    for (const auto& payment : payments) {
        double amount1 = payment.second;

        EXPECT_NE(amount1, amount2);
        EXPECT_DOUBLE_EQ(amount2, payments2[payment.first]);
        amount2 = payments2[payment.first];
    }
}

TEST(Instrument, EqualPaymentInstrumentDual) {
    TestSetup<dual> vars;
    EqualPaymentInstrument<dual> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.atlasRate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testInterest<dual>(prod, schedule, vars.atlasRate);

    const auto& leg         = prod.leg();
    const auto& coupons     = leg.coupons();
    const auto& redemptions = leg.redemptions();

    // test if payments are equal
    std::vector<Cashflow<dual>> cashflows;
    cashflows.insert(cashflows.end(), coupons.begin(), coupons.end());
    cashflows.insert(cashflows.end(), redemptions.begin(), redemptions.end());

    std::map<Date, double> payments;
    for (const auto& cashflow : cashflows) {
        if (payments.find(cashflow.paymentDate()) == payments.end()) { payments[cashflow.paymentDate()] = 0; }
        payments[cashflow.paymentDate()] += val(cashflow.amount());
    }

    double firstPayment = payments.begin()->second;
    for (const auto& payment : payments) {
        EXPECT_DOUBLE_EQ(firstPayment, payment.second);
        firstPayment = payment.second;
    }

    testChangeCurrency<EqualPaymentInstrument<dual>, dual>(prod);
}