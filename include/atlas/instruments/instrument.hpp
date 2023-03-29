#ifndef C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E
#define C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E

#include <atlas/cashflows/leg.hpp>  //fix

namespace Atlas {

    class Visitor;
    class ConstVisitor;

    /**
     * @class Instrument
     * @brief An abstract class for instruments.
     */
    class Instrument {
       public:
        /**
         * @brief Accepts a visitor.
         *
         * @param visitor
         */
        virtual void accept(Visitor& visitor) = 0;

        /**
         * @brief Accepts a const visitor.
         *
         * @param visitor
         */
        virtual void accept(ConstVisitor& visitor) const = 0;

        /**
         * @brief Returns the start date of the instrument.
         *
         * @return Date
         */
        inline Date startDate() const { return startDate_; };

        /**
         * @brief Returns the end date of the instrument.
         *
         * @return Date
         */
        inline Date endDate() const { return endDate_; };

        /**
         * @brief Returns the notional of the instrument.
         *
         * @return double
         */
        inline double notional() const { return notional_; };

       protected:
        Date startDate_  = Date();
        Date endDate_    = Date();
        double notional_ = 0.0;
    };
}  // namespace Atlas

#endif /* C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E */
