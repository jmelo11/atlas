#include "pch.hpp"
#include <ql/time/schedule.hpp>
#include <atlas/basictypes/rateindex.hpp>
#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletproduct.hpp>

using namespace Atlas;

TEST(Product, FloatingRateProduct) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional = 100;
    double spread   = 0.01;
    double fixing   = 0.03;
    LIBOR6M index;
    FloatingRateBulletProduct product(startDate, endDate, notional, spread, index);

    QuantLib::Schedule schedule = QuantLib::MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());

    auto& leg         = product.leg();
    auto& coupons     = leg.coupons();
    const auto& dates = schedule.dates();

    EXPECT_EQ(leg.discountCurve(), "undefined");
    EXPECT_EQ(leg.forecastCurve(), index.name());

    for (size_t i = 0; i < coupons.size(); ++i) {
        coupons[i].fwdIdx(i);
        coupons[i].fixing(fixing);
    }

    double yf = index.dayCounter().yearFraction(startDate, endDate);
    for (size_t i = 0; i < coupons.size(); ++i) {
        double yf       = (dates.at(i + 1) - dates.at(i)) / 360.0;
        double expected = notional * yf * (fixing + spread);
        double actual   = coupons[i].amount();
        EXPECT_FLOAT_EQ(expected, actual);
        EXPECT_EQ(coupons.at(i).startDate(), dates.at(i));
        EXPECT_EQ(coupons.at(i).endDate(), dates.at(i + 1));
    }
}