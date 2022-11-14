#include <atlas/cashflows/floatingratecoupon.hpp>

namespace Atlas {
    FloatingRateCoupon::FloatingRateCoupon(const QuantLib::Date& startDate,
                                           const QuantLib::Date& endDate, double notional,
                                           double spread, const RateIndex& index)
    : Coupon(startDate, endDate, notional), spread_(spread), index_(index){};

    double FloatingRateCoupon::accruedAmount(const QuantLib::Date& start,
                                             const QuantLib::Date& end) const {
        QuantLib::InterestRate rate(fixing_ + spread_, index_.dayCounter(),
                                    index_.rateCompounding(), index_.rateFrequency());
        return (rate.compoundFactor(start, end) - 1) * notional();
    };

    QuantLib::DayCounter FloatingRateCoupon::dayCounter() const {
        return index_.dayCounter();
    }

}  // namespace Atlas