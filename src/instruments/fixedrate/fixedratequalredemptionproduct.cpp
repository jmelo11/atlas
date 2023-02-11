#include <ql/time/schedule.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/fixedrate/fixedrateequalredemptionproduct.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FixedRateEqualRedemptionProduct::FixedRateEqualRedemptionProduct(const QuantLib::Date& startDate, const QuantLib::Date& endDate,
                                                                     QuantLib::Frequency freq, double notional, const QuantLib::InterestRate& rate)
    : FixedRateProduct(startDate, endDate, rate, notional) {
        QuantLib::Schedule schedule       = QuantLib::MakeSchedule().from(startDate).to(endDate).withFrequency(freq);
        std::vector<QuantLib::Date> dates = schedule.dates();
        std::vector<double> redemptions(dates.size() - 1, notional / (dates.size() - 1));

        double outstanding = notional;
        for (size_t i = 0; i < redemptions.size(); ++i) {
            FixedRateCoupon coupon(dates.at(i), dates.at(i + 1), outstanding, rate);
            leg_.addCoupon(coupon);
            
            Redemption redemption(dates.at(i + 1), redemptions.at(i));
            leg_.addRedemption(redemption);
            outstanding -= redemptions.at(i);
        }
    }

    void FixedRateEqualRedemptionProduct::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FixedRateEqualRedemptionProduct::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

}  // namespace Atlas
