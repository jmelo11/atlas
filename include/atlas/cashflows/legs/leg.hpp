#ifndef FCBEE32E_AB43_4796_A9DB_9E6108A27EF5
#define FCBEE32E_AB43_4796_A9DB_9E6108A27EF5

// #include <atlas/cashflows/cashflow.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Atlas {

    /**
     * @class Leg
     * @brief Represents a leg of a financial instrument, containing a vector of Redemption objects
     */
    template <typename adouble>
    class Leg {
       public:
        /**
         * @brief Default constructor
         */
        Leg() = default;

        /**
         * @brief Constructor that initializes a Leg object with a vector of Redemption objects
         * @param redemptions The vector of Redemption objects
         * @param sort Flag indicating whether to sort the redemptions by payment date
         */
        Leg(std::vector<Redemption<adouble>>& redemptions, bool sort = false) : redemptions_(redemptions) {
            if (sort) sortCashflows(redemptions_);
        };

        /**
         * @brief Default destructor
         */
        virtual ~Leg(){};

        /**
         * @brief Returns a reference to the vector of Redemption objects contained in the Leg object
         * @return Reference to the vector of Redemption objects
         */
        inline std::vector<Redemption<adouble>>& redemptions() { return redemptions_; }

        /**
         * @brief Returns a constant reference to the vector of Redemption objects contained in the Leg object
         * @return Constant reference to the vector of Redemption objects
         */
        inline const std::vector<Redemption<adouble>>& redemptions() const { return redemptions_; }

        /**
         * @brief Adds a Redemption object to the vector of redemptions contained in the Leg object
         * @param redemption Redemption object to be added
         */
        inline void addRedemption(Redemption<adouble>& redemption) { redemptions_.push_back(redemption); }

        /**
         * @brief Sorts the redemptions in the vector by payment date
         */
        inline void sortRedemptions() { sortCashflows(redemptions_); }

       protected:
        /**
         * @brief Template method that sorts a vector of cashflows by payment date
         * @tparam C Type of the cashflows to be sorted
         * @param cashflows The vector of cashflows to be sorted
         */

        template <typename C>
        void sortCashflows(std::vector<C>& cashflows) {
            auto f = [](const C& lhs, const C& rhs) { return lhs.paymentDate() < rhs.paymentDate(); };
            std::sort(cashflows.begin(), cashflows.end(), f);
        }

        template <typename C>
        void setDiscountCurveContext(std::vector<C>& cashflows, const CurveContext<adouble>& context) {
            for (auto& cashflow : cashflows) { cashflow.discountCurveContext(context); }
        }

        std::vector<Redemption<adouble>> redemptions_;  // Vector of Redemption objects contained in the Leg object
    };

    /**
     * @class Leg2, testing template parameters
     * @brief Testing template parameters, represents a leg of a financial instrument, containing a vector of Redemption objects
     */
    template <typename adouble, class Flow>
    class Leg2 {
       public:
        /**
         * @brief Default constructor
         */
        Leg2(){};

        /**
         * @brief Constructor that initializes a Leg object with a vector of Redemption objects
         * @param redemptions The vector of Redemption objects
         * @param sort Flag indicating whether to sort the redemptions by payment date
         */
        Leg2(std::vector<Flow>& coupons, std::vector<Redemption<adouble>>& redemptions, bool sorting = false)
        : coupons_(coupons), redemptions_(redemptions) {
            if (sorting) { sort(); }
        };

        /**
         * @brief Default destructor
         */
        virtual ~Leg2(){};

        /**
         * @brief Returns a reference to the vector of Redemption objects contained in the Leg object
         * @return Reference to the vector of Redemption objects
         */
        inline std::vector<Redemption<adouble>>& redemptions() { return redemptions_; }

        /**
         * @brief Returns a constant reference to the vector of Redemption objects contained in the Leg object
         * @return Constant reference to the vector of Redemption objects
         */
        inline const std::vector<Redemption<adouble>>& redemptions() const { return redemptions_; }

        /**
         * @brief Returns a reference to the vector of coupons in the leg.
         */
        inline std::vector<Flow>& coupons() { return coupons_; }

        /**
         * @brief Returns a const reference to the vector of coupons in the leg.
         */
        inline const std::vector<Flow>& coupons() const { return coupons_; }

        /**
         * @brief Adds a coupon to the leg.
         *
         * @param coupon a reference to a FloatingRateCoupon object representing the coupon to be added.
         * @param sort boolean indicating whether the cashflows should be sorted by date.
         */
        inline void addCoupon(Flow& coupon, bool sort = false) {
            coupons_.push_back(coupon);
            if (sort) sortCashflows(coupons_);
        }

        /**
         * @brief Sorts the cashflows in the leg by date.
         */
        inline void sort() {
            sortCashflows(redemptions_);
            sortCashflows(coupons_);
        }

        /**
         * @brief Sets the discount curve context for all cashflows in the leg.
         *
         * @param discountCurveContext a reference to a CurveContext object representing the discount curve context.
         */
        inline void discountCurveContext(const CurveContext<adouble>& discountCurveContext) {
            setDiscountCurveContext(coupons_, discountCurveContext);
            setDiscountCurveContext(redemptions_, discountCurveContext);
        }

        /**
         * @brief Sets the forecast curve context for all coupons in the leg.
         *
         * @tparam adouble
         * @tparam Flow
         * @param forecastCurveContext
         * @return void
         */
        std::enable_if_t<std::is_same_v<Flow, FloatingRateCoupon<adouble>>, void> inline forecastCurveContext(
            const CurveContext<adouble>& forecastCurveContext) {
            for (auto& coupon : coupons_) { coupon.forecastCurveContext(forecastCurveContext); }
        }

        /**
         * @brief Adds a Redemption object to the vector of redemptions contained in the Leg object
         * @param redemption Redemption object to be added
         */
        inline void addRedemption(Redemption<adouble>& redemption) { redemptions_.push_back(redemption); }

        /**
         * @brief Sorts the redemptions in the vector by payment date
         */
        inline void sortRedemptions() { sortCashflows(redemptions_); }

       protected:
        /**
         * @brief Template method that sorts a vector of cashflows by payment date
         * @tparam C Type of the cashflows to be sorted
         * @param cashflows The vector of cashflows to be sorted
         */

        void sortCashflows(std::vector<Flow>& cashflows) {
            auto f = [](const Flow& lhs, const Flow& rhs) { return lhs.paymentDate() < rhs.paymentDate(); };
            std::sort(cashflows.begin(), cashflows.end(), f);
        }

        void setDiscountCurveContext(std::vector<Flow>& cashflows, const CurveContext<adouble>& context) {
            for (auto& cashflow : cashflows) { cashflow.discountCurveContext(context); }
        }

        std::vector<Flow> coupons_;
        std::vector<Redemption<adouble>> redemptions_;  // Vector of Redemption objects contained in the Leg object
    };
}  // namespace Atlas

#endif /* FCBEE32E_AB43_4796_A9DB_9E6108A27EF5 */
