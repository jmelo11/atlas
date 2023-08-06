#ifndef C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E
#define C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E

namespace Atlas {

    template <typename adouble>
    class Visitor;

    template <typename adouble>
    class ConstVisitor;

    /**
     * @defgroup Instruments Instruments
     */

    /**
     * @class Instrument
     * @brief An abstract class for instruments.
     */
    template <typename adouble>
    class Instrument {
       public:
        Instrument() = default;

        Instrument(const Date& startDate, const Date& endDate, double notional = 0.0, Side side = Side::Recieve)
        : startDate_(startDate), endDate_(endDate), notional_(notional), side_(side) {}

        /**
         * @brief Returns the start date of the instrument.
         *
         * @return Date
         */
        inline const Date& startDate() const { return startDate_; };

        /**
         * @brief Returns the end date of the instrument.
         *
         * @return Date
         */
        inline const Date& endDate() const { return endDate_; };

        /**
         * @brief Returns the notional of the instrument.
         *
         * @return double
         */
        inline double notional() const { return notional_; };

        /**
         * @brief Returns the side of the instrument.
         *
         * @return Side
         */
        inline Side side() const { return side_; };

       protected:
        Date startDate_  = Date();
        Date endDate_    = Date();
        double notional_ = 0.0;
        Side side_       = Side::Recieve;
    };
}  // namespace Atlas

#endif /* C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E */
