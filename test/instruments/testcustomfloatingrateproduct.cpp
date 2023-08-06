#include "general.hpp"
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <numeric>

TEST(Instrument, CustomFloatingRateInstrument) {
    TestSetup<double> vars;

    Schedule schedule       = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.paymentFrequency);
    std::vector<Date> dates = schedule.dates();

    double redemption = 50;
    double notional   = redemption * (schedule.dates().size() - 1);
    std::map<Date, double> redemptionMap, disbursementMap;

    for (size_t i = 1; i < dates.size(); ++i) { redemptionMap[dates[i]] = redemption; }
    disbursementMap[dates[0]] = notional;

    auto& context = vars.store.curveManager().curveContext("TEST");
    auto& index   = context.index();
    CustomFloatingRateInstrument<double> prod(disbursementMap, redemptionMap, vars.spread, index);

    EXPECT_EQ(prod.cashflows().redemptionCount(), redemptionMap.size());
    EXPECT_EQ(prod.cashflows().disbursementCount(), disbursementMap.size());
    EXPECT_EQ(prod.cashflows().floatingRateCouponCount(), dates.size() - 1);

    for (const auto& r : prod.cashflows().redemptions()) { EXPECT_EQ(r.amount(), redemption); }
    for (const auto& d : prod.cashflows().disbursements()) { EXPECT_EQ(d.amount(), notional); }
}
