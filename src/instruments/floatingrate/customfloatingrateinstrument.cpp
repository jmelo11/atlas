#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>
#include <numeric>

namespace Atlas {
    CustomFloatingRateInstrument::CustomFloatingRateInstrument(std::vector<Date> dates, std::vector<double> redemptions, double spread)
    : FloatingRateInstrument(dates.front(), dates.back(), 0, spread) {
        notional_          = std::reduce(redemptions.begin(), redemptions.end());
        double outstanding = notional_;
        for (size_t i = 0; i < redemptions.size(); i++) {
            Redemption redemption(dates.at(i + 1), redemptions.at(i));
            leg_.addRedemption(redemption);

            FloatingRateCoupon coupon(dates.at(i), dates.at(i + 1), outstanding, spread);
            leg_.addCoupon(coupon);
            outstanding -= redemptions.at(i);
        }        
    };

}  // namespace Atlas