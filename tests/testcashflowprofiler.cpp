#include "pch.hpp"
#include <ql/time/schedule.hpp>
#include <atlas/visitors/cashflowprofiler.hpp>
#include <atlas/visitors/forwardrateforecaster.hpp>

using namespace Atlas;
namespace QL = QuantLib;

TEST(CashflowProfiler, Deposit) {
    QL::Date startDate(1, QL::January, 2018);
    QL::Date endDate(1, QL::January, 2019);
    double notional = 100.0;
    double rate     = 0.05;

    QL::InterestRate interestRate(rate, QL::Actual360(), QL::Compounded, QL::Annual);

    Deposit deposit(startDate, endDate, notional, interestRate);
    CashflowProfiler profiler;
    profiler.visit(deposit);

    const auto& interests   = profiler.interests();
    const auto& redemptions = profiler.redemptions();

    EXPECT_EQ(interests.at(deposit.endDate()), deposit.leg().coupons().at(0).amount());
    EXPECT_EQ(redemptions.at(deposit.endDate()), deposit.notional());
}

TEST(CashflowProfiler, EqualPaymentProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional          = 100;
    QuantLib::Frequency freq = QuantLib::Frequency::Semiannual;
    QuantLib::InterestRate rate(0.03, QuantLib::Actual360(), QuantLib::Simple, QuantLib::Annual);
    EqualPaymentProduct prod(startDate, endDate, freq, notional, rate);

    CashflowProfiler profiler;
    profiler.visit(prod);

    const auto& interestProfile   = profiler.interests();
    const auto& redemptionProfile = profiler.redemptions();

    const auto& coupons     = prod.leg().coupons();
    const auto& redemptions = prod.leg().redemptions();

    for (const auto& coupon : coupons) { EXPECT_EQ(interestProfile.at(coupon.paymentDate()), coupon.amount()); }
    for (const auto& redemption : redemptions) { EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemption.amount()); }
}

TEST(CashflowProfiler, CustomFixedRateProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);

    QuantLib::Frequency freq    = QuantLib::Frequency::Semiannual;
    QuantLib::Schedule schedule = QuantLib::MakeSchedule().from(startDate).to(endDate).withFrequency(freq);

    QuantLib::DayCounter dayCounter = QuantLib::Actual360();
    QuantLib::InterestRate rate(0.03, dayCounter, QuantLib::Compounding::Simple, QuantLib::Frequency::Annual);

    std::vector<double> redemptionAmounts(schedule.dates().size() - 1, 50);  // constant redemptions
    // auto notional = std::reduce(redemptionAmounts.begin(), redemptionAmounts.end());
    CustomFixedRateProduct prod(schedule.dates(), redemptionAmounts, rate);

    CashflowProfiler profiler;
    profiler.visit(prod);

    const auto& interestProfile   = profiler.interests();
    const auto& redemptionProfile = profiler.redemptions();

    const auto& coupons     = prod.leg().coupons();
    const auto& redemptions = prod.leg().redemptions();

    for (const auto& coupon : coupons) { EXPECT_EQ(interestProfile.at(coupon.paymentDate()), coupon.amount()); }
    for (const auto& redemption : redemptions) { EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemption.amount()); }
}

TEST(CashflowProfiler, FloatingRateBulletProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional = 100;
    double spread   = 0.01;
    double fixing   = 0.03;
    LIBOR6M index;
    FloatingRateBulletProduct prod(startDate, endDate, notional, spread, index);

    auto& coupons           = prod.leg().coupons();
    const auto& redemptions = prod.leg().redemptions();

    MarketData marketData;
    for (auto& coupon : coupons) {
        marketData.fwds.push_back(fixing);
        coupon.fwdIdx(marketData.fwds.size() - 1);
    }
    ForwardRateForecaster forecaster(marketData);
    forecaster.visit(prod);

    CashflowProfiler profiler;
    profiler.visit(prod);

    Profile interestProfile   = profiler.interests();
    Profile redemptionProfile = profiler.redemptions();

    for (const auto& coupon : coupons) { EXPECT_EQ(interestProfile.at(coupon.paymentDate()), coupon.amount()); }
    for (const auto& redemption : redemptions) { EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemption.amount()); }

    profiler.clear();

    prod.spread(0.0);
    profiler.visit(prod);
    Profile interestProfile2   = profiler.interests();
    Profile redemptionProfile2 = profiler.redemptions();

    for (const auto& coupon : coupons) {
        EXPECT_EQ(interestProfile2.at(coupon.paymentDate()), coupon.amount());
        EXPECT_NE(interestProfile.at(coupon.paymentDate()), interestProfile2.at(coupon.paymentDate()));
    }

    for (const auto& redemption : redemptions) {
        EXPECT_EQ(redemptionProfile2.at(redemption.paymentDate()), redemption.amount());
        EXPECT_EQ(redemptionProfile.at(redemption.paymentDate()), redemptionProfile2.at(redemption.paymentDate()));
    }
}