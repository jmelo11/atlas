#include <ql/time/schedule.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletproduct.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FixedRateBulletProduct::FixedRateBulletProduct(const QuantLib::Date& startDate,
                                           const QuantLib::Date& endDate, QuantLib::Frequency freq,
                                           double notional, const QuantLib::InterestRate& rate)
    : FixedRateProduct(startDate, endDate, rate, notional) {
        QuantLib::Schedule schedule =
            QuantLib::MakeSchedule().from(startDate).to(endDate).withFrequency(freq);

        QuantLib::Date firstDate = QuantLib::Date();
        for (const auto& lastDate : schedule.dates()) {
            if (firstDate != QuantLib::Date()) {
                FixedRateCoupon coupon(firstDate, lastDate, notional, rate);
                leg_.addCoupon(coupon);
            }
            firstDate = lastDate;
        }

        Redemption redemption(schedule.endDate(), notional);
        leg_.addRedemption(redemption);
    }

    void FixedRateBulletProduct::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FixedRateBulletProduct::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

}  // namespace Atlas