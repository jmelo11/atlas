#include <atlas/cashflows/coupon.hpp>

namespace Atlas {

    Coupon::Coupon(const QuantLib::Date& startDate, const QuantLib::Date& endDate, double notional)
    : Cashflow(), startDate_(startDate), endDate_(endDate), notional_(notional) {
        paymentDate_ = endDate;
    };

    double Coupon::notional() const {
        return notional_;
    }

    double Coupon::accruedPeriod(const QuantLib::Date& start, const QuantLib::Date& end) const {
        return dayCounter().yearFraction(start, end);
    }

}  // namespace Atlas
