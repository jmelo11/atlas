#ifndef A99BE6C5_A4B9_405A_995A_87CEAC4348E3
#define A99BE6C5_A4B9_405A_995A_87CEAC4348E3

#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/legs/leg.hpp>

namespace Atlas {

    class FixedRateLeg : public Leg {
       public:
        FixedRateLeg() : Leg(){};

        FixedRateLeg(std::vector<FixedRateCoupon> coupons, std::vector<Redemption> redemptions, bool sort = false)
        : Leg(redemptions, sort), coupons_(coupons) {
            if (sort) sortCashflows(coupons_);
        };

        inline std::vector<FixedRateCoupon>& coupons() { return coupons_; }

        inline const std::vector<FixedRateCoupon>& constCoupons() const { return coupons_; }

        inline void addCoupon(FixedRateCoupon& coupon, bool sort = false) {
            coupons_.push_back(coupon);
            if (sort) sortCashflows(coupons_);
        }

        inline void sort() {
            sortCashflows(redemptions_);
            sortCashflows(coupons_);
        }

        inline void discountCurveContext(const CurveContext& context) {
            setDiscountCurveContext(coupons_, context);
            setDiscountCurveContext(redemptions_, context);
        }

       private:
        std::vector<FixedRateCoupon> coupons_;
    };
}  // namespace Atlas

#endif /* A99BE6C5_A4B9_405A_995A_87CEAC4348E3 */
