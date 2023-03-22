#ifndef A53B011D_D929_446B_92A0_CDFF70167D4D
#define A53B011D_D929_446B_92A0_CDFF70167D4D

#include <atlas/atlasconfig.hpp>
#include <map>

namespace Atlas {

    /***
     * @brief A rate index.
     */
    class RateIndex {
       public:
        RateIndex();

        /***
         * @brief Constructs a rate index.
         * @param name The name of the rate index.
         * @param fixingFreq The fixing frequency.
         * @param dayCounter The day counter.
         * @param rateFreq The rate frequency.
         * @param rateComp The rate compounding.
         */
        RateIndex(const std::string& name, Frequency fixingFreq, DayCounter dayCounter = Actual360(), Frequency rateFreq = Frequency::Annual,
                  Compounding rateComp = Compounding::Simple);

        /***
         * @brief Returns the name of the rate index.
         */
        const std::string& name() const;

        /***
         * @brief Returns the day counter.
         */
        const DayCounter& dayCounter() const;

        /***
         * @brief Returns the fixing frequency.
         */
        Frequency fixingFrequency() const;

        /***
         * @brief Returns the rate frequency.
         */
        Frequency rateFrequency() const;

        /***
         * @brief Returns the rate compounding.
         */
        Compounding rateCompounding() const;

        /***
         * @brief Adds a fixing to the history.
         * @param date The date of the fixing.
         * @param value The value of the fixing.
         */
        void addFixing(const Date& date, double value);

        /***
         * @brief Returns the fixing for a given date.
         * @param date The date of the fixing.
         */
        double getFixing(const Date& date) const;

       private:
        std::string name_      = "undefined";
        DayCounter dayCounter_ = DayCounter();
        Frequency fixingFreq_  = Frequency::NoFrequency;
        Frequency rateFreq_    = Frequency::NoFrequency;
        Compounding rateComp_  = Compounding::Simple;
        std::map<Date, double> fixingHistory_;
    };

}  // namespace Atlas

#endif /* A53B011D_D929_446B_92A0_CDFF70167D4D */
