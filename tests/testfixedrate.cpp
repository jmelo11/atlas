#include "commonvars.hpp"
#include "pch.hpp"
#include <atlas/instruments/fixedrateinstrument.hpp>

TEST(TestDAPConsistency, TestFixedRate) {
    Date startDate(1, Month::Apr, 2022);
    Date endDate(1, Month::Apr, 2023);
    int days = endDate - startDate;
    InterestRate rate(0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    double amount   = 100;
    double interest = amount * days * rate / 360;
    Atlas::MetaData data;
    Atlas::DAP dap(startDate, endDate, amount, rate, data);
    dap.calculateInterests();  // explicit

    EXPECT_FLOAT_EQ(dap.faceAmount(), amount);
    EXPECT_FLOAT_EQ(dap.payment(), amount + interest);
    EXPECT_EQ(dap.outstanding().size(), 1);
    EXPECT_EQ(dap.interest().size(), 1);
}

TEST(TestFixedBulletConsistency, TestFixedRate) {
    DayCounter dayCounter = commonVars.dayCounter;

    double amount = 100;

    Atlas::FixedBullet bullet(commonVars.startDate, commonVars.endDate, commonVars.paymentFreq, commonVars.amount, commonVars.rate,
                              commonVars.metaData);
    Schedule schedule = MakeSchedule().from(commonVars.startDate).to(commonVars.endDate).withFrequency(commonVars.paymentFreq);

    bullet.calculateInterests();
    Date lastDate = commonVars.startDate;
    std::vector<double> coupons;
    for (const auto& d : schedule.dates()) {
        double yf = dayCounter.yearFraction(lastDate, d);
        if (yf > 0.0 + QL_EPSILON) { coupons.push_back(amount * (commonVars.rate.compoundFactor(yf) - 1)); }
        lastDate = d;
    }
    EXPECT_EQ(bullet.interest().size(), coupons.size());
    for (size_t i = 0; i < coupons.size(); i++) { EXPECT_FLOAT_EQ(bullet.interest().at(i), coupons.at(i)); }
}

TEST(TestEqualPaymentConsistency, TestFixedRate) {
    Atlas::EqualPayment equalPayment(commonVars.startDate, commonVars.endDate, commonVars.paymentFreq, commonVars.amount, commonVars.rate,
                                     commonVars.metaData);
    Schedule schedule = MakeSchedule().from(commonVars.startDate).to(commonVars.endDate).withFrequency(commonVars.paymentFreq);

    // check if payments are equal :|

    for (size_t i = 1; i < equalPayment.leg().size(); i++) {
        EXPECT_FLOAT_EQ(equalPayment.interest().at(i) + equalPayment.redemptions().at(i),
                        equalPayment.interest().at(0) + equalPayment.redemptions().at(0));
    }

    std::vector<Date> dates = schedule.dates();
    std::vector<double> coupons;
    equalPayment.calculateOutstanding();
    for (size_t i = 1; i < dates.size(); i++) {
        double value = (commonVars.rate.compoundFactor(dates[i - 1], dates[i]) - 1) * equalPayment.outstanding().at(i - 1);
        coupons.push_back(value);
    }
    EXPECT_EQ(equalPayment.interest().size(), coupons.size());
    for (size_t i = 0; i < coupons.size(); i++) { EXPECT_FLOAT_EQ(equalPayment.interest().at(i), coupons.at(i)); }
}

TEST(TestFixedCustomConsistency, TestFixedRate) {
    Schedule schedule = MakeSchedule().from(commonVars.startDate).to(commonVars.endDate).withFrequency(commonVars.paymentFreq);

    double notional         = 100;
    std::vector<Date> dates = schedule.dates();
    std::vector<double> notionals(schedule.size() - 1, notional / schedule.size());
    Atlas::FixedCustomStructure bond(dates, notionals, commonVars.rate, commonVars.metaData);
    bond.calculateOutstanding();

    for (size_t i = 0; i < bond.leg().size(); i++) { EXPECT_FLOAT_EQ(notional / schedule.size(), bond.redemptions().at(i)); }
}