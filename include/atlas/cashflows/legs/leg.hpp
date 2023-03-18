#ifndef FCBEE32E_AB43_4796_A9DB_9E6108A27EF5
#define FCBEE32E_AB43_4796_A9DB_9E6108A27EF5

#include <memory>
#include <string>
#include <vector>

namespace Atlas {

    /**
     * @class Leg
     * @brief Represents a leg of a financial instrument, containing a vector of Redemption objects
     */
    class Leg {
       public:
        /**
         * @brief Default constructor
         */
        Leg(){};

        /**
         * @brief Constructor that initializes a Leg object with a vector of Redemption objects
         * @param redemptions The vector of Redemption objects
         * @param sort Flag indicating whether to sort the redemptions by payment date
         */
        Leg(std::vector<Redemption> redemptions, bool sort = false) : redemptions_(redemptions) {
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
        std::vector<Redemption>& redemptions() { return redemptions_; }

        /**
         * @brief Returns a constant reference to the vector of Redemption objects contained in the Leg object
         * @return Constant reference to the vector of Redemption objects
         */
        const std::vector<Redemption>& constRedemptions() const { return redemptions_; }

        /**
         * @brief Adds a Redemption object to the vector of redemptions contained in the Leg object
         * @param redemption Redemption object to be added
         */
        void addRedemption(Redemption& redemption) { redemptions_.push_back(redemption); }

        /**
         * @brief Sorts the redemptions in the vector by payment date
         */
        void sortRedemptions() { sortCashflows(redemptions_); }

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

        std::vector<Redemption> redemptions_;  // Vector of Redemption objects contained in the Leg object
    };
}  // namespace Atlas

#endif /* FCBEE32E_AB43_4796_A9DB_9E6108A27EF5 */
