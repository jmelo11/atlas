
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/fixedrateproduct.hpp>
#include <atlas/visitors/visitor.hpp>
#include <map>
namespace Atlas {
    FixedRateProduct::FixedRateProduct(const QuantLib::Date& startDate,
                                       const QuantLib::Date& endDate,
                                       const QuantLib::InterestRate& rate, double notional,
                                       const FixedRateLeg& leg)
    : leg_(leg), rate_(rate) {
        startDate_ = startDate;
        endDate_   = endDate;
        notional_  = notional;
    };
    
    void FixedRateProduct::calculateNotionals(const std::vector<QuantLib::Date>& dates,
                                              const QuantLib::InterestRate& rate) {
        std::map<QuantLib::Date, double> notionals;
        double notional = 0.0;
        for (const auto& redemption : leg_.redemptions()) {
            notional += redemption.amount();
            notionals[redemption.paymentDate()] = redemption.amount();
        }

        for (size_t i = 0; i < dates.size() - 1; i++) {
            FixedRateCoupon coupon(dates[i], dates[i + 1], notional, rate);
            leg_.addCoupon(coupon);
            if (notionals.find(dates[i + 1]) != notionals.end())
                notional -= notionals[dates[i + 1]];
        }
    }

    void FixedRateProduct::calculateFaceAmount() {
        double faceAmount = 0.0;
        for (const auto& redemption : leg_.redemptions()) { faceAmount += redemption.amount(); }
        notional_ = faceAmount;
    }

    void FixedRateProduct::rate(const QuantLib::InterestRate& r) {
        rate_ = r;
        for (auto& coupon : leg_.coupons()) { coupon.rate(r); }
    }

    void FixedRateProduct::rate(double r) {
        QuantLib::InterestRate tmpR(r, rate_.dayCounter(), rate_.compounding(), rate_.frequency());
        rate(tmpR);
    }

    FixedRateInstrument::FixedRateInstrument(const QuantLib::Date& startDate,
                                       const QuantLib::Date& endDate,
                                       const QuantLib::InterestRate& rate, double notional,
                                       const FixedRateLeg2& leg)
    : leg_(leg), rate_(rate) {
        startDate_ = startDate;
        endDate_   = endDate;
        notional_  = notional;
    };
    
    void FixedRateInstrument::calculateNotionals(const std::vector<QuantLib::Date>& dates,
                                              const QuantLib::InterestRate& rate) {
        std::map<QuantLib::Date, double> notionals;
        double notional = 0.0;
        for (const auto& redemption : leg_.redemptions()) {
            notional += redemption.amount();
            notionals[redemption.paymentDate()] = redemption.amount();
        }

        for (size_t i = 0; i < dates.size() - 1; i++) {
            FixedRateCoupon coupon(dates[i], dates[i + 1], notional, rate);
            leg_.addCoupon(coupon);
            if (notionals.find(dates[i + 1]) != notionals.end())
                notional -= notionals[dates[i + 1]];
        }
    }

    void FixedRateInstrument::calculateFaceAmount() {
        double faceAmount = 0.0;
        for (const auto& redemption : leg_.redemptions()) { faceAmount += redemption.amount(); }
        notional_ = faceAmount;
    }

    void FixedRateInstrument::rate(const QuantLib::InterestRate& r) {
        rate_ = r;
        for (auto& coupon : leg_.coupons()) { coupon.rate(r); }
    }

    void FixedRateInstrument::rate(double r) {
        QuantLib::InterestRate tmpR(r, rate_.dayCounter(), rate_.compounding(), rate_.frequency());
        rate(tmpR);
    }

    void FixedRateInstrument::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FixedRateInstrument::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }
}  // namespace Atlas