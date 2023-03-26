#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/rates/curvecontextstore.hpp>

namespace Atlas {
    FloatingRateCoupon::FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, double spread,
                                           const CurveContext& forecastCurveContext)
    : Coupon(startDate, endDate, notional), spread_(spread), forecastContextIdx_(forecastCurveContext.idx()), hasForecastContext_(true) {
        rateDef_ = {forecastCurveContext.index().dayCounter(), forecastCurveContext.index().rateFrequency(),
                    forecastCurveContext.index().rateCompounding()};
    };

    FloatingRateCoupon::FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, double spread,
                                           const CurveContext& forecastCurveContext, const CurveContext& discountCurveContext)
    : FloatingRateCoupon(startDate, endDate, notional, spread, forecastCurveContext) {
        discountContextIdx_ = discountCurveContext.idx();
        hasDiscountContext_ = true;
    }

}  // namespace Atlas