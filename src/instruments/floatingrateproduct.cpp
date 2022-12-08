
#include <atlas/instruments/floatingrateproduct.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FloatingRateProduct::FloatingRateProduct(const QuantLib::Date& startDate, const QuantLib::Date& endDate, double notional, double spread,
                                             const FloatingRateLeg& leg)
    : leg_(leg), spread_(spread) {
        startDate_ = startDate;
        endDate_   = endDate;
        notional_  = notional;
    };

    void FloatingRateProduct::spread(double s) {
        spread_ = s;
        for (auto& coupon : leg_.coupons()) { coupon.spread(s); }
    }
}  // namespace Atlas