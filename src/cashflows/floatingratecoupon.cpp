#include <atlas/cashflows/floatingratecoupon.hpp>

namespace Atlas {
    FloatingRateCoupon::FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, double spread,
                                           std::shared_ptr<CurveContext> discountCurveContext, std::shared_ptr<CurveContext> forecastCurveContext)
    : Coupon(startDate, endDate, notional, discountCurveContext), spread_(spread), forecastCurveContext_(forecastCurveContext){};

    void FloatingRateCoupon::fixing(double fixing) {
        fixing_           = fixing;
        const auto& index = forecastCurveContext_->index();
        InterestRate r(fixing_ + spread_, index.dayCounter(), index.rateCompounding(), index.rateFrequency());
        amount_ = notional() * r.compoundFactor(startDate(), endDate());
    }

    DayCounter FloatingRateCoupon::dayCounter() const {
        return forecastCurveContext_->index().dayCounter();
    }

    double FloatingRateCoupon::accruedPeriod(const Date& start, const Date& end) const {
        return dayCounter().yearFraction(start, end);
    }

    double FloatingRateCoupon::accruedAmount(const Date& start, const Date& end) const {
        const auto& index = forecastCurveContext_->index();
        InterestRate r(fixing_ + spread_, index.dayCounter(), index.rateCompounding(), index.rateFrequency());
        return notional() * (r.compoundFactor(start, end) - 1.0);
    }
}  // namespace Atlas