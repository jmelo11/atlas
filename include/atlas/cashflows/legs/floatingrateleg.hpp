#ifndef C06DA827_18B3_4441_9F99_0EA936D79F04
#define C06DA827_18B3_4441_9F99_0EA936D79F04

#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/cashflows/legs/leg.hpp>

namespace Atlas {
    /**
     * @brief A class representing a floating rate leg, which is a collection of floating rate coupons
     * @details A floating rate leg has a start date, and contains one or more floating rate coupons
     */
    class FloatingRateLeg : public Leg {
       public:
        /**
         * @brief Default constructor.
         */
        FloatingRateLeg() : Leg(){};

        /**
         * @brief Constructor.
         *
         * @param coupons vector of FloatingRateCoupon objects representing the coupons in the leg.
         * @param redemptions vector of Redemption objects representing the redemptions in the leg.
         * @param sort boolean indicating whether the cashflows should be sorted by date.
         */
        FloatingRateLeg(std::vector<FloatingRateCoupon> coupons, std::vector<Redemption> redemptions, bool sort = false)
        : Leg(redemptions, sort), coupons_(coupons) {
            if (sort) sortCashflows(coupons_);
        };

        /**
         * @brief Returns a reference to the vector of coupons in the leg.
         */
        std::vector<FloatingRateCoupon>& coupons() { return coupons_; }

        /**
         * @brief Returns a const reference to the vector of coupons in the leg.
         */
        const std::vector<FloatingRateCoupon>& constCoupons() const { return coupons_; }

        /**
         * @brief Adds a coupon to the leg.
         *
         * @param coupon a reference to a FloatingRateCoupon object representing the coupon to be added.
         * @param sort boolean indicating whether the cashflows should be sorted by date.
         */
        void addCoupon(FloatingRateCoupon& coupon, bool sort = false) {
            coupons_.push_back(coupon);
            if (sort) sortCashflows(coupons_);
        }

        /**
         * @brief Sorts the cashflows in the leg by date.
         */
        void sort() {
            sortCashflows(redemptions_);
            sortCashflows(coupons_);
        }

        void discountCurveContext(std::shared_ptr<CurveContext> discountCurveContext) {
            setDiscountCurveContext(coupons_, discountCurveContext);
            setDiscountCurveContext(redemptions_, discountCurveContext);
        }

        void forecastCurveContext(std::shared_ptr<CurveContext> forecastCurveContext) {
            for (auto& coupon : coupons_) { coupon.forecastCurveContext(forecastCurveContext); }
        }

       private:
        std::vector<FloatingRateCoupon> coupons_; /**< vector of FloatingRateCoupon objects representing the coupons in the leg. */
    };

}  // namespace Atlas

#endif /* C06DA827_18B3_4441_9F99_0EA936D79F04 */
