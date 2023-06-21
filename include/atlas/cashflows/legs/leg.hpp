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
     * @defgroup Legs Legs
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
         *
         * @param redemptions The vector of Redemption objects
         * @param sort Flag indicating whether to sort the redemptions by payment date
         */
        Leg(std::vector<Redemption<adouble>>& redemptions, bool sort = false) : redemptions_(redemptions) {
            if (sort) sortCashflows(redemptions_);
        };

        /**
         * @brief Default destructor
         *
         */
        virtual ~Leg(){};

        /**
         * @brief Returns a reference to the vector of Redemption objects contained in the Leg object
         *
         * @return Reference to the vector of Redemption objects
         */
        inline std::vector<Redemption<adouble>>& redemptions() { return redemptions_; }

        /**
         * @brief Returns a constant reference to the vector of Redemption objects contained in the Leg object
         *
         * @return Constant reference to the vector of Redemption objects
         */
        inline const std::vector<Redemption<adouble>>& redemptions() const { return redemptions_; }

        /**
         * @brief Adds a Redemption object to the vector of redemptions contained in the Leg object
         *
         * @param redemption Redemption object to be added
         * @return void
         */
        inline void addRedemption(Redemption<adouble>& redemption) { redemptions_.push_back(redemption); }

        /**
         * @brief Sorts the redemptions in the vector by payment date
         *
         * @return void
         */
        inline void sortRedemptions() { sortCashflows(redemptions_); }

       protected:
        /**
         * @brief Template method that sorts a vector of cashflows by payment date
         * @tparam C Type of the cashflows to be sorted
         *
         * @param cashflows The vector of cashflows to be sorted
         */
        template <typename C>
        void sortCashflows(std::vector<C>& cashflows) {
            auto f = [](const C& lhs, const C& rhs) { return lhs.paymentDate() < rhs.paymentDate(); };
            std::sort(cashflows.begin(), cashflows.end(), f);
        }

        /**
         * @brief Set the Discount Curve Context object
         *
         * @tparam C Type of the cashflows
         * @param cashflows Vector of cashflows
         * @param context Discount curve context
         */
        template <typename C>
        void setDiscountCurveContext(std::vector<C>& cashflows, const CurveContext<adouble>& context) {
            for (auto& cashflow : cashflows) { cashflow.discountCurveContext(context); }
        }

        std::vector<Redemption<adouble>> redemptions_;  // Vector of Redemption objects contained in the Leg object
    };

}  // namespace Atlas

#endif /* FCBEE32E_AB43_4796_A9DB_9E6108A27EF5 */
