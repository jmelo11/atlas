#include <atlas/cashflows/floatingratecoupon.hpp>

namespace Atlas {
    FloatingRateCoupon::FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, double spread, const RateIndex& index)
    : Coupon(startDate, endDate, notional), spread_(spread), index_(index){};

    double FloatingRateCoupon::accruedAmount(const Date& start, const Date& end) const {
        InterestRate rate(fixing_ + spread_, index_.dayCounter(), index_.rateCompounding(), index_.rateFrequency());
        return (rate.compoundFactor(start, end) - 1) * notional();
    };

    DayCounter FloatingRateCoupon::dayCounter() const {
        return index_.dayCounter();
    }

    double FloatingRateCoupon2::accruedAmount(const Date& start, const Date& end) const {
        InterestRate rate(fixing_ + spread_, index().dayCounter(), index().rateCompounding(), index().rateFrequency());
        return (rate.compoundFactor(start, end) - 1) * notional();
    };

    DayCounter FloatingRateCoupon2::dayCounter() const {
        return index().dayCounter();
    }

}  // namespace Atlas