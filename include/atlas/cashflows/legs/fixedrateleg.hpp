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

        void discountCurveContext(std::shared_ptr<CurveContext> context) {
            setDiscountCurveContext(coupons_, context);
            setDiscountCurveContext(redemptions_, context);
        }

        std::shared_ptr<CurveContext> discountCurveContext() const { return coupons_.front().discountCurveContext(); }

       private:
        std::vector<FixedRateCoupon> coupons_;
    };
}  // namespace Atlas

#endif /* A99BE6C5_A4B9_405A_995A_87CEAC4348E3 */
