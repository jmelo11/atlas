#ifndef A1CBC2C8_49CC_41E1_9304_45973FAD8B80
#define A1CBC2C8_49CC_41E1_9304_45973FAD8B80

#include <atlas/rates/interestrate.hpp>
#include <atlas/rates/yieldtermstructure/yieldtermstructure.hpp>

namespace Atlas {

    /**
     * @class DiscountTermStructure
     * @brief This class uses discount factors to construct a yield term structure.
     *
     * @tparam adouble
     * @tparam Interpol<adouble>
     */
    template <template <typename> class Interpol, typename adouble = double>
    class DiscountTermStructure : public YieldTermStructure<adouble> {
       private:
        /**
         * @brief The strategy holds the methods that provide all the calculations for the given object.
         */
        class Strategy : public YieldTermStructure<adouble>::Strategy {
           public:
            explicit Strategy(const std::vector<Date>& dates, const std::vector<adouble>& dfs, const DayCounter& dayCounter)
            : YieldTermStructure<adouble>::Strategy(), dayCounter_(dayCounter), dates_(dates), dfs_(dfs) {
                if (dates.size() != dfs.size()) { throw std::invalid_argument("dates and dfs must have the same size"); }
                if (dates.size() < 2) { throw std::invalid_argument("dates and dfs must have at least 2 elements"); }
                times_    = std::vector<double>(dates.size());
                times_[0] = 0.0;

                for (size_t i = 1; i < dates.size(); ++i) {
                    if (dates[i] < dates[i - 1]) { throw std::invalid_argument("dates must be sorted"); }
                    double t = dayCounter.yearFraction(dates[0], dates[i]);
                    if (t < 0) throw std::invalid_argument("dates must be greater than reference date");
                    times_[i] = t;
                }
                this->refDate_ = dates[0];
                interpol_      = Interpol<adouble>(times_, dfs_);
            }

            /**
             * @brief Gets the interpolated discount factor for the given date.
             *
             * @param date
             * @return adouble
             */
            adouble discount(const Date& date) const override {
                double t = dayCounter_.yearFraction(this->refDate_, date);
                if (t < 0) throw std::invalid_argument("date must be greater than reference date");
                return interpol_(t);
            };

            /**
             * @brief Gets the interpolated discount factor for the given time fraction.
             *
             * @param date
             * @return adouble
             */
            adouble discount(double t) const override { return interpol_(t); };

            /**
             * @brief Gets the interpolated forward rate for the given dates.
             *
             * @param date
             * @return adouble
             */
            adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp,
                                Frequency freq) const override {
                double t1     = dayCounter_.yearFraction(this->refDate_, startDate);
                double t2     = dayCounter_.yearFraction(this->refDate_, endDate);
                adouble value = interpol_(t1) / interpol_(t2);
                auto r        = InterestRate<adouble>::impliedRate(value, dayCounter, comp, freq, startDate, endDate);
                return r.rate();
            };
            
            /**
             * @brief Clones the object.
             *
             * @return std::unique_ptr<typename YieldTermStructure<adouble>::Strategy>
             */
            std::unique_ptr<typename YieldTermStructure<adouble>::Strategy> clone() const override {
                return std::make_unique<Strategy>(dates_, dfs_, dayCounter_);
            };

            /**
             * @brief Enables/disables extrapolation.
             * @details If disabled, an exception will be raise if times are greather than the discounts max date.
             * @param e
             */
            void enableExtrapolation(bool e) override { interpol_.enableExtrapolation(e); };

           private:
            DayCounter dayCounter_;
            std::vector<double> times_;
            std::vector<Date> dates_;
            std::vector<adouble> dfs_;
            Interpol<adouble> interpol_;
        };

       public:
        /**
         * @brief Construct a new Discount Term Structure object
         * @details First date defines the reference date of the curve.
         * @param dates 
         * @param dfs 
         * @param dayCounter 
         */
        explicit DiscountTermStructure(const std::vector<Date>& dates, const std::vector<adouble>& dfs, const DayCounter& dayCounter)
        : YieldTermStructure<adouble>(std::make_unique<Strategy>(dates, dfs, dayCounter)){};
    };
}  // namespace Atlas

#endif /* A1CBC2C8_49CC_41E1_9304_45973FAD8B80 */
