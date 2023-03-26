#ifndef A67E2CE6_E4F1_4688_AD7C_AB8C6EF2ADE1
#define A67E2CE6_E4F1_4688_AD7C_AB8C6EF2ADE1

#include <ql/time/schedule.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/curves/rateindex.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FloatingRateBulletInstrument::FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, double notional, double spread,
                                                               const CurveContext& forecastCurveContext)
    : FloatingRateInstrument(startDate, endDate, notional, spread) {
        const auto& index = forecastCurveContext.index();
        Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());
        Date firstDate    = Date();
        for (const auto& endDate : schedule.dates()) {
            if (firstDate != Date()) {
                FloatingRateCoupon coupon(firstDate, endDate, notional, spread, forecastCurveContext);
                leg_.addCoupon(coupon);
            }
            firstDate = endDate;
        }

        Redemption redemption(schedule.endDate(), notional);
        leg_.addRedemption(redemption);

        disbursement_ = Cashflow(startDate, -notional);
    }

    FloatingRateBulletInstrument::FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, double notional, double spread,
                                                               const CurveContext& forecastCurveContext, const CurveContext& discountCurveContext)
    : FloatingRateBulletInstrument(startDate, endDate, notional, spread, forecastCurveContext) {
        leg().discountCurveContext(discountCurveContext);
        disbursement_.discountCurveContext(discountCurveContext);
    }
}  // namespace Atlas

#endif /* A67E2CE6_E4F1_4688_AD7C_AB8C6EF2ADE1 */
