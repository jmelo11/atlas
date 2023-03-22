#include <atlas/cashflows/coupon.hpp>

namespace Atlas {

    Coupon::Coupon(const Date& startDate, const Date& endDate, double notional, std::shared_ptr<CurveContext> discountCurveContext)
    : Cashflow(discountCurveContext), startDate_(startDate), endDate_(endDate), notional_(notional) {
        paymentDate_ = endDate;  // paymentDate shouldnt be same as endDate
    };

    double Coupon::notional() const {
        return notional_;
    }

}  // namespace Atlas
