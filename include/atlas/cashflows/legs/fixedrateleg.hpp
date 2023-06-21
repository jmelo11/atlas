#ifndef A99BE6C5_A4B9_405A_995A_87CEAC4348E3
#define A99BE6C5_A4B9_405A_995A_87CEAC4348E3

#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/legs/leg.hpp>

namespace Atlas {

    /**
     * @class FixedRateLeg
     * @brief Fixed rate leg class, hold multiple fixed rate coupons and redemptions.
     * @ingroup Legs
     *
     * @tparam adouble
     */
    template <typename adouble>
    class FixedRateLeg : public Leg<adouble> {
       public:
        FixedRateLeg() : Leg<adouble>(){};

        /**
         * @brief Construct a new Fixed Rate Leg object
         *
         * @param coupons A vector of fixed rate coupons.
         * @param redemptions A vector of redemptions.
         * @param sort If true, the cashflows will be sorted by date.
         */
        FixedRateLeg(std::vector<FixedRateCoupon<adouble>>& coupons, std::vector<Redemption<adouble>>& redemptions, bool sort = false)
        : Leg<adouble>(redemptions, sort), coupons_(coupons) {
            if (sort) this->sortCashflows(coupons_);
        };

        /**
         * @brief Construct a new Fixed Rate Leg object
         *
         * @return std::vector<FixedRateCoupon<adouble>>&
         */
        inline std::vector<FixedRateCoupon<adouble>>& coupons() { return coupons_; }

        /**
         * @brief Get the coupons object
         *
         * @return const std::vector<FixedRateCoupon<adouble>>&
         */
        inline const std::vector<FixedRateCoupon<adouble>>& coupons() const { return coupons_; }

        /**
         * @brief Add a fixed rate coupon to the leg.
         *
         * @param coupon A fixed rate coupon.
         * @param sort If true, the cashflows will be sorted by date.
         */
        inline void addCoupon(FixedRateCoupon<adouble>& coupon, bool sort = false) {
            coupons_.push_back(coupon);
            if (sort) this->sortCashflows(coupons_);
        }

        /**
         * @brief Sort the cashflows by date.
         *
         */
        inline void sort() {
            this->sortCashflows(this->redemptions_);
            this->sortCashflows(coupons_);
        }

        /**
         * @brief Sets the discount curve context for the leg.
         *
         * @param discountCurveContext a reference to a Context object representing the discount curve context.
         */
        inline void discountCurveContext(const CurveContext<adouble>& context) {
            this->setDiscountCurveContext(this->redemptions_, context);
            this->setDiscountCurveContext(coupons_, context);
        }

       private:
        std::vector<FixedRateCoupon<adouble>> coupons_;
    };
}  // namespace Atlas

#endif /* A99BE6C5_A4B9_405A_995A_87CEAC4348E3 */
