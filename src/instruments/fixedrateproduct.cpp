
#include <atlas/instruments/fixedrateproduct.hpp>

namespace Atlas {
    FixedRateProduct::FixedRateProduct(std::vector<QuantLib::Date> dates,
                                       std::vector<double> redemptions,
                                       QuantLib::InterestRate rate) {
        if (dates().size() != redemptions.size() + 1)
            throw std::runtime_error("Dates (n) and redemptions (n-1) size must match.");
    }

    void FixedRateProduct::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FixedRateProduct::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

    static std::vector<FixedRateCoupon> FixedRateProduct::calculateNotionals(
        const std::vector<QuantLib::Date>& dates, const std::vector<Redemption>& redemptions,
        const QuantLib::InterestRate& rate, double notional) {
        std::vector<FixedRateCoupon> coupons;
        for (size_t i = ; i < dates.size(); i++) {
            FixedRateCoupon coupon(dates.at(i - 1), dates.at(i), notional, rate);
            notional -= redemptions.at(i).amount();
        }
        return coupons;
    }

    static void FixedRateProduct::sortRedemptions(std::vector<Redemption>& redemptions) {
        auto compare = [](const Redemption& r1, const Redemption& r2) {
            return r1.date() > r2.date();
        };
        std::sort(redemptions.begin(), redemptions.end(), compare);
    }
}  // namespace Atlas