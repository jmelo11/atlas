#include "pch.hpp"
#include <ql/time/daycounters/actual360.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/instruments/fixedrate/equalpaymentproduct.hpp>

using namespace Atlas;

TEST(Product, EqualPaymentProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional          = 100;
    QuantLib::Frequency freq = QuantLib::Frequency::Semiannual;
    QuantLib::InterestRate rate(0.03, QuantLib::Actual360(), QuantLib::Simple, QuantLib::Annual);
    EqualPaymentProduct prod(startDate, endDate, freq, notional, rate);

    const auto& leg         = prod.constLeg();
    const auto& coupons     = leg.constCoupons();
    const auto& redemptions = leg.constRedemptions();

    EXPECT_EQ(leg.discountCurve(), "undefined");

    // test if payments are equal
    std::vector<Cashflow> cashflows;
    cashflows.insert(cashflows.end(), coupons.begin(), coupons.end());
    cashflows.insert(cashflows.end(), redemptions.begin(), redemptions.end());

    std::map<QuantLib::Date, double> payments;
    for (const auto& cashflow : cashflows) {
        if (payments.find(cashflow.paymentDate()) == payments.end()) {
            payments[cashflow.paymentDate()] = 0;
        }
        payments[cashflow.paymentDate()] += cashflow.amount();
    }

    double firstPayment = payments.begin()->second;
    for (const auto& payment : payments) {
        EXPECT_FLOAT_EQ(firstPayment, payment.second);
        firstPayment = payment.second;
    }
}