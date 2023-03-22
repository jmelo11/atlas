#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {

    ZeroCouponInstrument::ZeroCouponInstrument(const Date& startDate, const Date& endDate, double notional, const InterestRate& rate,
                                               std::shared_ptr<CurveContext> discountCurveContext)
    : FixedRateInstrument(startDate, endDate, rate, notional) {
        FixedRateCoupon coupon(startDate, endDate, notional, rate, discountCurveContext);  // interest coupon
        Redemption redemption(endDate, notional, discountCurveContext);                    // notinal payment at the end

        leg_.addCoupon(coupon);
        leg_.addRedemption(redemption);

        disbursement_ = Cashflow(startDate, -notional, discountCurveContext);
    }

}  // namespace Atlas