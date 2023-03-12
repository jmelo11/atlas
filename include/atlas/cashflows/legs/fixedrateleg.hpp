#ifndef A99BE6C5_A4B9_405A_995A_87CEAC4348E3
#define A99BE6C5_A4B9_405A_995A_87CEAC4348E3

#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/legs/leg.hpp>

namespace Atlas {
    class FixedRateLeg : public Leg {
       public:
        FixedRateLeg() : Leg(){};

        FixedRateLeg(std::vector<FixedRateCoupon> coupons, std::vector<Redemption> redemptions) : Leg(redemptions), coupons_(coupons) {
            startDate_ = coupons_.at(0).startDate();
        };

        std::vector<FixedRateCoupon>& coupons() { return coupons_; }

        const std::vector<FixedRateCoupon>& constCoupons() const { return coupons_; }

        void addCoupon(FixedRateCoupon& coupon) {
            coupons_.push_back(coupon);
            if (startDate() == Date() || startDate() > coupon.startDate()) { startDate_ = coupon.startDate(); }
        }

       private:
        std::vector<FixedRateCoupon> coupons_;
    };

    class FixedRateLeg2 : public Leg2 {
       public:
        FixedRateLeg2() : Leg2(){};

        FixedRateLeg2(std::vector<FixedRateCoupon> coupons, std::vector<Redemption2> redemptions, bool sort = false)
        : Leg2(redemptions, sort), coupons_(coupons) {
            if (sort) sortCashflows(coupons_);
        };

        std::vector<FixedRateCoupon>& coupons() { return coupons_; }

        const std::vector<FixedRateCoupon>& constCoupons() const { return coupons_; }

        void addCoupon(FixedRateCoupon& coupon, bool sort = false) {
            coupons_.push_back(coupon);
            if (sort) sortCashflows(coupons_);
        }

        void sort() {
            sortCashflows(redemptions_);
            sortCashflows(coupons_);
        }

        void discountCurve(size_t idx) {
            for (auto& coupon : coupons_) { coupon.discountCurveIdx(idx); }
            for (auto& redemptions_ : redemptions_) { redemptions_.discountCurveIdx(idx); }
        }

       private:
        std::vector<FixedRateCoupon> coupons_;
    };
}  // namespace Atlas

#endif /* A99BE6C5_A4B9_405A_995A_87CEAC4348E3 */
