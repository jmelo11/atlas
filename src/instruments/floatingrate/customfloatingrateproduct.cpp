#include <atlas/instruments/floatingrate/customfloatingrateproduct.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    CustomFloatingRateProduct::CustomFloatingRateProduct(std::vector<QuantLib::Date> dates, std::vector<double> redemptions, double spread,
                                                         const RateIndex& index)
    : FloatingRateProduct(dates.front(), dates.back(), 0, spread) {
        for (size_t i = 0; i < redemptions.size(); i++) {
            Redemption redemption(dates.at(i + 1), redemptions.at(i));
            leg_.addRedemption(redemption);

            FloatingRateCoupon coupon(dates.at(i), dates.at(i + 1), redemptions.at(i), spread, index);
            leg_.addCoupon(coupon);
            notional_ += redemptions.at(i);
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