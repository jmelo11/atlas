#ifndef C8964333_99C5_4B58_B60E_1881A7E5EE1F
#define C8964333_99C5_4B58_B60E_1881A7E5EE1F

#include <atlas/rates/interestrate.hpp>
#include <atlas/rates/yieldtermstructure/yieldtermstructure.hpp>

namespace Atlas {

    /**
     * @class FlatForwardTermStructure
     * @brief This class provides functionality for dummy curves.
     * @details It assumes a constant flat-forward rate.
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class FlatForwardTermStructure : public YieldTermStructure<adouble> {
       private:
        /**
         * @brief The strategy holds the methods that provide all the calculations for the given object.
         */
        class Strategy : public YieldTermStructure<adouble>::Strategy {
           public:
            explicit Strategy(const Date& refDate, adouble rate, const DayCounter& dayCounter = Actual360(), Compounding comp = Compounding::Simple,
                              Frequency freq = Frequency::Annual)
            : YieldTermStructure<adouble>::Strategy(refDate), rate_(InterestRate<adouble>(rate, dayCounter, comp, freq)) {}

            explicit Strategy(const Date& refDate, const InterestRate<adouble>& rate) : YieldTermStructure<adouble>::Strategy(refDate), rate_(rate) {}

            /**
             * @brief Gets the interpolated discount factor for the given date.
             *
             * @param date
             * @return adouble
             */
            adouble discount(const Date& date) const override {
                if (date < this->refDate_) { throw std::invalid_argument("Date is before reference date"); }
                double t = rate_.dayCounter().yearFraction(this->refDate_, date);
                return discount(t);
            };

            /**
             * @brief Gets the interpolated discount factor for the given time fraction.
             *
             * @param date
             * @return adouble
             */
            adouble discount(double t) const override {
                adouble df = 1 / rate_.compoundFactor(t);
                return df;
            };

            /**
             * @brief Gets the interpolated forward rate for the given dates.
             *
             * @param date
             * @return adouble
             */
            adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp,
                                Frequency freq) const override {
                if (startDate < this->refDate_) { throw std::invalid_argument("Start date is before reference date"); }
                adouble compound = discount(startDate) / discount(endDate);
                return InterestRate<adouble>::impliedRate(compound, dayCounter, comp, freq, startDate, endDate).rate();
            };

            /**
             * @brief Clones the object.
             *
             * @return std::unique_ptr<typename YieldTermStructure<adouble>::Strategy>
             */
            std::unique_ptr<typename YieldTermStructure<adouble>::Strategy> clone() const override { return std::make_unique<Strategy>(*this); };

            /**
             * @brief Enables/disables extrapolation.
             * @details If disabled, an exception will be raise if times are greather than the discounts max date.
             * @param e
             */
            void enableExtrapolation(bool e) override { throw std::invalid_argument("Extrapolation not supported"); };

           private:
            InterestRate<adouble> rate_;
        };

       public:
        /**
         * @brief Construct a new Flat Forward Term Structure object
         *
         * @param refDate
         * @param rate
         * @param dayCounter
         * @param comp
         * @param freq
         */
        explicit FlatForwardTermStructure(const Date& refDate, adouble rate, const DayCounter& dayCounter = Actual360(),
                                          Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual)
        : YieldTermStructure<adouble>(std::make_unique<Strategy>(refDate, rate, dayCounter, comp, freq)){};

        /**
         * @brief Construct a new Flat Forward Term Structure object
         *
         * @param refDate
         * @param rate
         */
        explicit FlatForwardTermStructure(const Date& refDate, const InterestRate<adouble>& rate)
        : YieldTermStructure<adouble>(std::make_unique<Strategy>(refDate, rate)){};
    };
}  // namespace Atlas

#endif /* C8964333_99C5_4B58_B60E_1881A7E5EE1F */
