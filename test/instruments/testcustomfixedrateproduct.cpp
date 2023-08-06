#include "general.hpp"
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <numeric>

TEST(Instrument, CustomFixedRateInstrument) {
    TestSetup<double> vars;
    Schedule schedule       = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    std::vector<Date> dates = schedule.dates();

    double redemption = 50;
    double notional   = redemption * (schedule.dates().size() - 1);
    std::map<Date, double> redemptionMap, disbursementMap;

    for (size_t i = 1; i < dates.size(); ++i) { redemptionMap[dates[i]] = redemption; }
    disbursementMap[dates[0]] = notional;

    CustomFixedRateInstrument<double> prod(disbursementMap, redemptionMap, vars.atlasRate);

    EXPECT_EQ(prod.cashflows().redemptionCount(), redemptionMap.size());
    EXPECT_EQ(prod.cashflows().disbursementCount(), disbursementMap.size());
    EXPECT_EQ(prod.cashflows().fixedRateCouponCount(), dates.size() - 1);

    for (const auto& r : prod.cashflows().redemptions()) { EXPECT_EQ(r.amount(), redemption); }
    for (const auto& d : prod.cashflows().disbursements()) { EXPECT_EQ(d.amount(), notional); }

}
