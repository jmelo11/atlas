#ifndef A67E2CE6_E4F1_4688_AD7C_AB8C6EF2ADE1
#define A67E2CE6_E4F1_4688_AD7C_AB8C6EF2ADE1

#include <ql/time/schedule.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/curves/rateindex.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletproduct.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FloatingRateBulletProduct::FloatingRateBulletProduct(const QuantLib::Date& startDate,
                                                         const QuantLib::Date& endDate,
                                                         double notional, double spread,
                                                         RateIndex index)
    : FloatingRateProduct(startDate, endDate) {
        QuantLib::Schedule schedule =
            QuantLib::MakeSchedule().from(startDate).to(endDate).withFrequency(
                index.fixingFrequency());

        QuantLib::Date firstDate = QuantLib::Date();
        for (const auto& endDate : schedule.dates()) {
            if (firstDate != QuantLib::Date()) {
                FloatingRateCoupon coupon(firstDate, endDate, notional, spread, index);
                leg_.addCoupon(coupon);
            }
            firstDate = endDate;
        }

        Redemption redemption(schedule.endDate(), notional);
        leg_.addRedemption(redemption);
    }

    void FloatingRateBulletProduct::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FloatingRateBulletProduct::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

}  // namespace Atlas

#endif /* A67E2CE6_E4F1_4688_AD7C_AB8C6EF2ADE1 */
