#include <ql/time/schedule.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/curves/rateindex.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FloatingRateEqualRedemptionInstrument::FloatingRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, double notional,
                                                                                 double spread, std::shared_ptr<CurveContext> forecastCurveContext,
                                                                                 std::shared_ptr<CurveContext> discountCurveContext)
    : FloatingRateInstrument(startDate, endDate, notional, spread) {
        const auto& index = forecastCurveContext->index();
        Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());
        const auto& dates = schedule.dates();
        std::vector<double> redemptions(schedule.size() - 1, notional / (schedule.size() - 1));

        double outstanding = notional;
        for (size_t i = 0; i < dates.size() - 1; ++i) {
            FloatingRateCoupon coupon(dates.at(i), dates.at(i + 1), outstanding, spread, forecastCurveContext, discountCurveContext);
            leg_.addCoupon(coupon);
            Redemption redemption(dates.at(i + 1), redemptions.at(i), discountCurveContext);
            leg_.addRedemption(redemption);
            outstanding -= redemptions.at(i);
        }

        disbursement_ = Cashflow(startDate, -notional, discountCurveContext);
    };

}  // namespace Atlas
