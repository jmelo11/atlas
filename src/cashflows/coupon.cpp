#include <atlas/cashflows/coupon.hpp>

namespace Atlas {

    double Coupon::notional() const {
        return notional_;
    }

    double Coupon::accruedPeriod(const QuantLib::Date& start, const QuantLib::Date& end) const {
        return dayCounter().yearFraction(start, end);
    }

    QuantLib::Date Coupon::date() const {
        return endDate();
    }

    bool Coupon::hasOcurred(const QuantLib::Date& date) const {
        if (endDate() > date) return false;
        return true;
    }

}  // namespace Atlas
