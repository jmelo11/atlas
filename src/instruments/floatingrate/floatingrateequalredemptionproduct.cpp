#include <ql/time/schedule.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/curves/rateindex.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptionproduct.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FloatingRateEqualRedemptionProduct::FloatingRateEqualRedemptionProduct(const QuantLib::Date& startDate, const QuantLib::Date& endDate,
                                                                           double notional, double spread, const RateIndex& index)
    : FloatingRateProduct(startDate, endDate, notional, spread) {
        QuantLib::Schedule schedule = QuantLib::MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());
        const auto& dates           = schedule.dates();
        std::vector<double> redemptions(schedule.size() - 1, notional / (schedule.size() - 1));
        
        double outstanding = notional;
        for (size_t i = 0; i < dates.size() - 1; ++i) {
            FloatingRateCoupon coupon(dates.at(i), dates.at(i+1), outstanding, spread, index);
            leg_.addCoupon(coupon);
            Redemption redemption(dates.at(i+1), redemptions.at(i));
            leg_.addRedemption(redemption);
            outstanding -= redemptions.at(i);
        }

        forecastCurve(index.name());
    };

    void FloatingRateEqualRedemptionProduct::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FloatingRateEqualRedemptionProduct::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

}  // namespace Atlas
