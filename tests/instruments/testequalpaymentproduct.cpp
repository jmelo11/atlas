#include "../pch.hpp"
#include "general.hpp"
#include <ql/time/daycounters/actual360.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>

TEST(Instrument, EqualPaymentInstrument) {
    FixedInstrumentVars vars;
    EqualPaymentInstrument prod(vars.startDate, vars.endDate, vars.paymentFrequency, vars.notional, vars.rate);
    Schedule schedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    testInterest(prod, schedule, vars.rate);

    const auto& leg         = prod.constLeg();
    const auto& coupons     = leg.constCoupons();
    const auto& redemptions = leg.constRedemptions();

    // test if payments are equal
    std::vector<Cashflow> cashflows;
    cashflows.insert(cashflows.end(), coupons.begin(), coupons.end());
    cashflows.insert(cashflows.end(), redemptions.begin(), redemptions.end());

    std::map<Date, double> payments;
    for (const auto& cashflow : cashflows) {
        if (payments.find(cashflow.paymentDate()) == payments.end()) { payments[cashflow.paymentDate()] = 0; }
        payments[cashflow.paymentDate()] += Value(cashflow.amount());
    }

    double firstPayment = payments.begin()->second;
    for (const auto& payment : payments) {
        EXPECT_FLOAT_EQ(firstPayment, payment.second);
        firstPayment = payment.second;
    }
}