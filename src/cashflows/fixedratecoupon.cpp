
#include <atlas/cashflows/fixedratecoupon.hpp>

namespace Atlas {

    FixedRateCoupon::FixedRateCoupon(const QuantLib::Date& startDate, const QuantLib::Date& endDate,
                                     double notional, const QuantLib::InterestRate& rate)
    : Coupon(startDate, endDate, notional), rate_(rate) {
        amount_ = accruedAmount(startDate, endDate);
    };

    double FixedRateCoupon::accruedAmount(const QuantLib::Date& start,
                                          const QuantLib::Date& end) const {
        return notional() * (rate_.compoundFactor(start, end) - 1.0);
    }

    QuantLib::DayCounter FixedRateCoupon::dayCounter() const {
        return rate_.dayCounter();
    }

    QuantLib::InterestRate FixedRateCoupon::rate() const {
        return rate_;
    }

    void FixedRateCoupon::rate(const QuantLib::InterestRate& rate) {
        rate_   = rate;
        amount_ = accruedAmount(startDate(), endDate());
    }

}  // namespace Atlas