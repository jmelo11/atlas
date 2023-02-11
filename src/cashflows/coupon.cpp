#include <atlas/cashflows/coupon.hpp>

namespace Atlas {

    Coupon::Coupon(const Date& startDate, const Date& endDate, double notional)
    : Cashflow(), startDate_(startDate), endDate_(endDate), notional_(notional) {
        paymentDate_ = endDate; // paymentDate shouldnt be same as endDate
    };

    double Coupon::notional() const {
        return notional_;
    }

    double Coupon::accruedPeriod(const Date& start, const Date& end) const {
        return dayCounter().yearFraction(start, end);
    }

}  // namespace Atlas
