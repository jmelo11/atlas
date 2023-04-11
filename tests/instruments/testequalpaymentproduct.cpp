#include "../pch.hpp"
#include "general.hpp"
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>

TEST(Instrument, EqualPaymentInstrument) {
    FixedInstrumentVars<double> vars;
    EqualPaymentInstrument<double> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testInterest<double>(prod, schedule, vars.rate);

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
        EXPECT_FLOAT_EQ(firstPayment, payment.second);
        firstPayment = payment.second;
    }
}

TEST(Instrument, EqualPaymentInstrumentDual) {
    FixedInstrumentVars<dual> vars;
    EqualPaymentInstrument<dual> prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testInterest<dual>(prod, schedule, vars.rate);

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
        EXPECT_FLOAT_EQ(firstPayment, payment.second);
        firstPayment = payment.second;
    }
}