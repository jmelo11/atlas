
#include <atlas/cashflows/fixedratecoupon.hpp>

namespace Atlas {

    FixedRateCoupon::FixedRateCoupon(const Date& startDate, const Date& endDate, double notional, const InterestRate& rate)
    : Coupon(startDate, endDate, notional), rate_(rate) {
        amount_ = Value(accruedAmount(startDate, endDate));
    };

    FixedRateCoupon::FixedRateCoupon(const Date& startDate, const Date& endDate, double notional, const InterestRate& rate,
                                     const CurveContext& discountCurveContext)
    : Coupon(startDate, endDate, notional, discountCurveContext), rate_(rate) {
        amount_ = Value(accruedAmount(startDate, endDate));
    };

}  // namespace Atlas