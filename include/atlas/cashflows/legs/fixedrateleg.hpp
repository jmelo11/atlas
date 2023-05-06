#ifndef A99BE6C5_A4B9_405A_995A_87CEAC4348E3
#define A99BE6C5_A4B9_405A_995A_87CEAC4348E3

#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/legs/leg.hpp>

namespace Atlas {

    /***
     * Fixed rate leg class
     */
    template <typename adouble>
    class FixedRateLeg : public Leg<adouble> {
       public:
        FixedRateLeg() : Leg<adouble>(){};

        FixedRateLeg(std::vector<FixedRateCoupon<adouble>>& coupons, std::vector<Redemption<adouble>>& redemptions, bool sort = false)
        : Leg<adouble>(redemptions, sort), coupons_(coupons) {
            if (sort) this->sortCashflows(coupons_);
        };

        inline std::vector<FixedRateCoupon<adouble>>& coupons() { return coupons_; }

        inline const std::vector<FixedRateCoupon<adouble>>& coupons() const { return coupons_; }

        inline void addCoupon(FixedRateCoupon<adouble>& coupon, bool sort = false) {
            coupons_.push_back(coupon);
            if (sort) this->sortCashflows(coupons_);
        }

        inline void sort() {
            this->sortCashflows(this->redemptions_);
            this->sortCashflows(coupons_);
        }

        inline void discountCurveContext(const CurveContext<adouble>& context) {
            this->setDiscountCurveContext(this->redemptions_, context);
            this->setDiscountCurveContext(coupons_, context);
        }

       private:
        std::vector<FixedRateCoupon<adouble>> coupons_;
    };
}  // namespace Atlas

#endif /* A99BE6C5_A4B9_405A_995A_87CEAC4348E3 */
