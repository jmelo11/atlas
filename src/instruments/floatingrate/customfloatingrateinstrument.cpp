#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>
#include <numeric>

namespace Atlas {
    CustomFloatingRateInstrument::CustomFloatingRateInstrument(std::vector<Date> dates, std::vector<double> redemptions, double spread,
                                                               const CurveContext& forecastCurveContext, const CurveContext& discountCurveContext)
    : CustomFloatingRateInstrument(dates, redemptions, spread, forecastCurveContext) {
        leg().discountCurveContext(discountCurveContext);
        disbursement_.discountCurveContext(discountCurveContext);
    };

    CustomFloatingRateInstrument::CustomFloatingRateInstrument(std::vector<Date> dates, std::vector<double> redemptions, double spread,
                                                               const CurveContext& forecastCurveContext)
    : FloatingRateInstrument(dates.front(), dates.back(), 0, spread) {
        notional_          = std::reduce(redemptions.begin(), redemptions.end());
        double outstanding = notional_;
        for (size_t i = 0; i < redemptions.size(); i++) {
            Redemption redemption(dates.at(i + 1), redemptions.at(i));
            leg_.addRedemption(redemption);

            FloatingRateCoupon coupon(dates.at(i), dates.at(i + 1), outstanding, spread, forecastCurveContext);
            leg_.addCoupon(coupon);
            outstanding -= redemptions.at(i);
        }

        disbursement_ = Cashflow(dates.front(), -notional_);
    };

}  // namespace Atlas