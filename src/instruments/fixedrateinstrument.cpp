
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>
#include <map>
namespace Atlas {

    FixedRateInstrument::FixedRateInstrument(const Date& startDate, const Date& endDate, const InterestRate& rate, double notional,
                                             const FixedRateLeg& leg)
    : leg_(leg), rate_(rate) {
        startDate_ = startDate;
        endDate_   = endDate;
        notional_  = notional;
    };

    void FixedRateInstrument::calculateNotionals(const std::vector<Date>& dates, const InterestRate& rate) {
        std::map<Date, double> notionals;
        double notional = 0.0;
        for (const auto& redemption : leg_.redemptions()) {
            notional += redemption.amount();
            notionals[redemption.paymentDate()] = redemption.amount();
        }
        notional_ = notional;
        for (size_t i = 0; i < dates.size() - 1; i++) {
            FixedRateCoupon coupon(dates[i], dates[i + 1], notional, rate);
            leg_.addCoupon(coupon);
            if (notionals.find(dates[i + 1]) != notionals.end()) notional -= notionals[dates[i + 1]];
        }
    }

    void FixedRateInstrument::rate(const InterestRate& r) {
        rate_ = r;
        for (auto& coupon : leg_.coupons()) { coupon.rate(r); }
    }

    void FixedRateInstrument::rate(double r) {
        InterestRate tmpR(r, rate_.dayCounter(), rate_.compounding(), rate_.frequency());
        rate(tmpR);
    }

    void FixedRateInstrument::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FixedRateInstrument::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }
}  // namespace Atlas