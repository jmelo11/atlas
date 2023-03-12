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
         * @brief Default constructor for FloatingRateLeg
         */
        FloatingRateLeg() : Leg(){};

        /**
         * @brief Constructor for FloatingRateLeg that takes a vector of coupons and redemptions
         * @param coupons A vector of floating rate coupons
         * @param redemptions A vector of redemptions
         */
        FloatingRateLeg(std::vector<FloatingRateCoupon> coupons, std::vector<Redemption> redemptions) : Leg(redemptions), coupons_(coupons) {
            startDate_ = coupons_.at(0).startDate();
        };

        /**
         * @brief Returns a reference to the vector of coupons
         * @return A reference to the vector of coupons
         */
        std::vector<FloatingRateCoupon>& coupons() { return coupons_; }

        /**
         * @brief Returns a const reference to the vector of coupons
         * @return A const reference to the vector of coupons
         */
        const std::vector<FloatingRateCoupon>& constCoupons() const { return coupons_; }

        /**
         * @brief Adds a coupon to the vector of coupons
         * @param coupon A reference to the coupon to add
         */
        void addCoupon(FloatingRateCoupon& coupon) {
            coupons_.push_back(coupon);
            if (startDate() == Date() || startDate() > coupon.startDate()) { startDate_ = coupon.startDate(); }
        }

        /**
         * @brief Sets the forecast curve for the floating rate leg
         * @param curve The name of the curve to use for forecasting
         */
        void forecastCurve(const std::string& curve) { forecastCurve_ = curve; }

        /**
         * @brief Returns the name of the forecast curve used by the floating rate leg
         * @return The name of the forecast curve used by the floating rate leg
         */
        const std::string& forecastCurve() const { return forecastCurve_; }

       private:
        std::vector<FloatingRateCoupon> coupons_; /**< A vector of floating rate coupons */
        std::string forecastCurve_ = "undefined"; /**< The name of the curve used for forecasting */
    };

    /**
     * @brief Represents a leg with floating rate coupons.
     */
    class FloatingRateLeg2 : public Leg2 {
       public:
        /**
         * @brief Default constructor.
         */
        FloatingRateLeg2() : Leg2(){};

        /**
         * @brief Constructor.
         *
         * @param coupons vector of FloatingRateCoupon2 objects representing the coupons in the leg.
         * @param redemptions vector of Redemption2 objects representing the redemptions in the leg.
         * @param sort boolean indicating whether the cashflows should be sorted by date.
         */
        FloatingRateLeg2(std::vector<FloatingRateCoupon2> coupons, std::vector<Redemption2> redemptions, bool sort = false)
        : Leg2(redemptions, sort), coupons_(coupons) {
            if (sort) sortCashflows(coupons_);
        };

        /**
         * @brief Returns a reference to the vector of coupons in the leg.
         */
        std::vector<FloatingRateCoupon2>& coupons() { return coupons_; }

        /**
         * @brief Returns a const reference to the vector of coupons in the leg.
         */
        const std::vector<FloatingRateCoupon2>& constCoupons() const { return coupons_; }

        /**
         * @brief Adds a coupon to the leg.
         *
         * @param coupon a reference to a FloatingRateCoupon2 object representing the coupon to be added.
         * @param sort boolean indicating whether the cashflows should be sorted by date.
         */
        void addCoupon(FloatingRateCoupon2& coupon, bool sort = false) {
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

        /**
         * @brief Sets the discount curve index for the leg's cashflows and coupons.
         *
         * @param idx size_t representing the index of the discount curve.
         */
        void discountCurveIdx(size_t idx) {
            for (auto& coupon : coupons_) { coupon.discountCurveIdx(idx); }
            for (auto& redemptions_ : redemptions_) { redemptions_.discountCurveIdx(idx); }
        }

        /**
         * @brief Sets the forecast curve index for the leg's coupons.
         *
         * @param idx size_t representing the index of the forecast curve.
         */
        void forecastCurveIdx(size_t idx) {
            for (auto& coupon : coupons_) { coupon.forecastCurveIdx(idx); }
        }

       private:
        std::vector<FloatingRateCoupon2> coupons_; /**< vector of FloatingRateCoupon2 objects representing the coupons in the leg. */
    };

}  // namespace Atlas

#endif /* C06DA827_18B3_4441_9F99_0EA936D79F04 */
