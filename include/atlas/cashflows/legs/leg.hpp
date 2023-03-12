#ifndef FCBEE32E_AB43_4796_A9DB_9E6108A27EF5
#define FCBEE32E_AB43_4796_A9DB_9E6108A27EF5

#include <atlas/cashflows/redemption.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Atlas {

    class Leg : public Indexable {
       public:
        Leg(){};

        Leg(std::vector<Redemption> redemptions) : redemptions_(redemptions){};

        virtual ~Leg(){};

        void discountCurve(const std::string& curve) { discountCurve_ = curve; }

        const std::string& discountCurve() const { return discountCurve_; }

        std::vector<Redemption>& redemptions() { return redemptions_; }

        const std::vector<Redemption>& constRedemptions() const { return redemptions_; }

        void addRedemption(Redemption& redemption) { redemptions_.push_back(redemption); }

        Date startDate() const { return startDate_; }

       protected:
        Date startDate_ = Date();

       private:
        std::vector<Redemption> redemptions_;
        std::string discountCurve_ = "undefined";
        size_t dfIdx_              = 0;  // this should be changed for the numeraire
    };

    /**
     * @class Leg2
     * @brief Represents a leg of a financial instrument, containing a vector of Redemption2 objects
     */
    class Leg2 {
       public:
        /**
         * @brief Default constructor
         */
        Leg2(){};

        /**
         * @brief Constructor that initializes a Leg2 object with a vector of Redemption2 objects
         * @param redemptions The vector of Redemption2 objects
         * @param sort Flag indicating whether to sort the redemptions by payment date
         */
        Leg2(std::vector<Redemption2> redemptions, bool sort = false) : redemptions_(redemptions) {
            if (sort) sortCashflows(redemptions_);
        };

        /**
         * @brief Default destructor
         */
        virtual ~Leg2(){};

        /**
         * @brief Returns a reference to the vector of Redemption2 objects contained in the Leg2 object
         * @return Reference to the vector of Redemption2 objects
         */
        std::vector<Redemption2>& redemptions() { return redemptions_; }

        /**
         * @brief Returns a constant reference to the vector of Redemption2 objects contained in the Leg2 object
         * @return Constant reference to the vector of Redemption2 objects
         */
        const std::vector<Redemption2>& constRedemptions() const { return redemptions_; }

        /**
         * @brief Adds a Redemption2 object to the vector of redemptions contained in the Leg2 object
         * @param redemption Redemption2 object to be added
         */
        void addRedemption(Redemption2& redemption) { redemptions_.push_back(redemption); }

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

        std::vector<Redemption2> redemptions_;  // Vector of Redemption2 objects contained in the Leg2 object
    };
}  // namespace Atlas

#endif /* FCBEE32E_AB43_4796_A9DB_9E6108A27EF5 */
