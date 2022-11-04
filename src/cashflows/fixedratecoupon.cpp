
#include <atlas/cashflows/fixedratecoupon.hpp>

namespace Atlas {

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

    void FixedRateCoupon::rate(QuantLib::InterestRate rate) {
        rate_ = rate;
    }

    double FixedRateCoupon::amount() const {
        return accruedAmount(startDate(), endDate());
    }

    

}  // namespace Atlas