#include <atlas/cashflows/floatingratecoupon.hpp>

namespace Atlas {
    FloatingRateCoupon::FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, double spread)
    : Coupon(startDate, endDate, notional), spread_(spread){};

}  // namespace Atlas