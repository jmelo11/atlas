#include <atlas/instruments/floatingrate/customfloatingrateproduct.hpp>
#include <atlas/visitors/visitor.hpp>
#include <numeric>

namespace Atlas {
    CustomFloatingRateProduct::CustomFloatingRateProduct(std::vector<Date> dates, std::vector<double> redemptions, double spread,
                                                         const RateIndex& index)
    : FloatingRateProduct(dates.front(), dates.back(), 0, spread) {
        notional_ = std::reduce(redemptions.begin(), redemptions.end());
        double outstanding = notional_;
        for (size_t i = 0; i < redemptions.size(); i++) {
            Redemption redemption(dates.at(i + 1), redemptions.at(i));
            leg_.addRedemption(redemption);

            FloatingRateCoupon coupon(dates.at(i), dates.at(i + 1), outstanding, spread, index);
            leg_.addCoupon(coupon);
            outstanding -= redemptions.at(i);
        }

        forecastCurve(index.name());
    };

    void CustomFloatingRateProduct::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void CustomFloatingRateProduct::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

}  // namespace Atlas