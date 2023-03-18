#ifndef A67E2CE6_E4F1_4688_AD7C_AB8C6EF2ADE1
#define A67E2CE6_E4F1_4688_AD7C_AB8C6EF2ADE1

#include <ql/time/schedule.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/curves/rateindex.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FloatingRateBulletInstrument::FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, double notional, double spread,
                                                         const RateIndexConfiguration& index)
    : FloatingRateInstrument(startDate, endDate, notional, spread) {
        Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());

        Date firstDate = Date();
        for (const auto& endDate : schedule.dates()) {
            if (firstDate != Date()) {
                FloatingRateCoupon coupon(firstDate, endDate, notional, spread);
                leg_.addCoupon(coupon);
            }
            firstDate = endDate;
        }

        Redemption redemption(schedule.endDate(), notional);
        leg_.addRedemption(redemption);

    }

}  // namespace Atlas

#endif /* A67E2CE6_E4F1_4688_AD7C_AB8C6EF2ADE1 */
