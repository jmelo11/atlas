#ifndef C06DA827_18B3_4441_9F99_0EA936D79F04
#define C06DA827_18B3_4441_9F99_0EA936D79F04

#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/cashflows/legs/leg.hpp>

namespace Atlas {
    /**
     * @class FloatingRateLeg
     * @brief A class representing a floating rate leg, which is a collection of floating rate coupons
     * @ingroup Legs
     * @tparam adouble
     */
    template <typename adouble>
    class FloatingRateLeg : public Leg<adouble> {
       public:
        /**
         * @brief Default constructor.
         */
        FloatingRateLeg() : Leg<adouble>(){};

        /**
         * @brief Construct a new Floating Rate Leg object
         *
         * @param coupons vector of FloatingRateCoupon objects representing the coupons in the leg.
         * @param redemptions vector of Redemption objects representing the redemptions in the leg.
         * @param sort boolean indicating whether the cashflows should be sorted by date.
         */
        FloatingRateLeg(std::vector<FloatingRateCoupon<adouble>>& coupons, std::vector<Redemption<adouble>>& redemptions, bool sort = false)
        : Leg<adouble>(redemptions, sort), coupons_(coupons) {
            if (sort) this->sortCashflows(coupons_);
        };

        /**
         * @brief Returns a reference to the vector of coupons in the leg.
         *
         * @return std::vector<FloatingRateCoupon<adouble>>&
         */
        inline std::vector<FloatingRateCoupon<adouble>>& coupons() { return coupons_; }

        /**
         * @brief Returns a const reference to the vector of coupons in the leg.
         *
         * @return const std::vector<FloatingRateCoupon<adouble>>&
         */
        inline const std::vector<FloatingRateCoupon<adouble>>& coupons() const { return coupons_; }

        /**
         * @brief Returns a const reference to the coupon at index i.
         *
         * @param i the index of the coupon.
         * @return const FloatingRateCoupon<adouble>&
         */
        inline const FloatingRateCoupon<adouble>& coupon(size_t i) const { return coupons_.at(i); }

        /**
         * @brief Returns a reference to the coupon at index i.
         *
         * @param i the index of the coupon.
         * @return FloatingRateCoupon<adouble>&
         */
        inline FloatingRateCoupon<adouble>& coupon(size_t i) { return coupons_[i]; }

        /**
         * @brief Adds a coupon to the leg.
         *
         * @param coupon a reference to a FloatingRateCoupon object representing the coupon to be added.
         * @param sort boolean indicating whether the cashflows should be sorted by date.
         */
        inline void addCoupon(FloatingRateCoupon<adouble>& coupon, bool sort = false) {
            coupons_.push_back(coupon);
            if (sort) this->sortCashflows(coupons_);
        }

        /**
         * @brief Sorts the cashflows in the leg by date.
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
        inline void discountCurveContext(const Context<YieldTermStructure<adouble>>& discountCurveContext) {
            this->setDiscountCurveContext(coupons_, discountCurveContext);
            this->setDiscountCurveContext(this->redemptions_, discountCurveContext);
        }

        /**
         * @brief Sets the forecast curve context for the leg.
         *
         * @param forecastCurveContext a reference to a Context object representing the forecast curve context.
         */
        inline void rateIndexContext(const Context<RateIndex<adouble>>& rateIndexContext) {
            for (auto& coupon : coupons_) { coupon.rateIndexContext(rateIndexContext); }
        }

       private:
        std::vector<FloatingRateCoupon<adouble>> coupons_; /**< vector of FloatingRateCoupon objects representing the coupons in the leg. */
    };

}  // namespace Atlas

#endif /* C06DA827_18B3_4441_9F99_0EA936D79F04 */
