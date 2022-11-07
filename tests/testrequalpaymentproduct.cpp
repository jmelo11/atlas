#include "pch.hpp"
#include <atlas/instruments/equalpaymentproduct.hpp>

TEST(TestEqualPaymentProduct, EqualPaymentProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional = 100;
    QuantLib::InterestRate rate(0.03, QuantLib::Actual360(), QuantLib::Simple, QuantLib::Annual);
    EqualPaymentProduct prod(startDate, endDate, notional, rate);

    const Leg& tmp = prod.constLeg(1);

    EXPECT_EQ(tmp.discountCurve(), "undefined");
    EXPECT_EQ(tmp.forecastCurve(), "undefined");

    for (const auto& cashflow : tmp.cashflows()) {
        FixedRateCoupon* ptr = std::dynamic_pointer_cast<FixedRateCoupon>(cashflow);
        if (ptr) {
            double yf          = rate.dayCounter().yearFraction(ptr->startDate(), ptr->endDate());
            double outstanding = ptr->notional();
            double interest    = yf * outstanding * rate.rate();
            EXPECT_FLOAT_EQ(interest, ptr->amount());
        }
    }

    // test if payments are equal
    std::map<QuantLib::Date, double> payments;
    for (const auto& cashflow : tmp.cashflows()) {
        if (payments.find(cashflow.date()) == payments.end()) { payments[cashflow.date()] = 0; }
        payments[cashflow.date()] += cashflow.amount();
    }

    for (auto iter = payments.begin(), iter != payments.end() - 1, iter++)
        EXPECT_FLOAT_EQ(iter, iter + 1);
}