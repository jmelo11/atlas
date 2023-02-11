
#include <atlas/cashflows/fixedratecoupon.hpp>

namespace Atlas {

    FixedRateCoupon::FixedRateCoupon(const Date& startDate, const Date& endDate, double notional, const InterestRate& rate)
    : Coupon(startDate, endDate, notional), rate_(rate) {
        amount_ = accruedAmount(startDate, endDate);
    };

    double FixedRateCoupon::accruedAmount(const Date& start, const Date& end) const {
        return notional() * (rate_.compoundFactor(start, end) - 1.0);
    }

    DayCounter FixedRateCoupon::dayCounter() const {
        return rate_.dayCounter();
    }

    InterestRate FixedRateCoupon::rate() const {
        return rate_;
    }

    void FixedRateCoupon::rate(const InterestRate& rate) {
        rate_   = rate;
        amount_ = accruedAmount(startDate(), endDate());
    }

}  // namespace Atlas