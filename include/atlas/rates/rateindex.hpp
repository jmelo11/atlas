#ifndef A53B011D_D929_446B_92A0_CDFF70167D4D
#define A53B011D_D929_446B_92A0_CDFF70167D4D

#include <atlas/atlasconfig.hpp>
#include <map>

namespace Atlas {

    /**
     * @brief A rate index.
     */
    class RateIndex {
       public:
        RateIndex();

        /**
         * @brief Constructs a rate index.
         * @param name The name of the rate index.
         * @param fixingFreq The fixing frequency.
         * @param dayCounter The day counter.
         * @param rateFreq The rate frequency.
         * @param rateComp The rate compounding.
         */
        RateIndex(const std::string& name, Frequency fixingFreq, DayCounter dayCounter = Actual360(), Frequency rateFreq = Frequency::Annual,
                  Compounding rateComp = Compounding::Simple);

        /**
         * @brief Returns the name of the rate index.
         */
        inline const std::string& name() const { return name_; };

        /**
         * @brief Returns the day counter.
         */
        inline const DayCounter& dayCounter() const { return dayCounter_; };

        /**
         * @brief Returns the fixing frequency.
         */
        inline Frequency fixingFrequency() const { return fixingFreq_; };

        /**
         * @brief Returns the rate frequency.
         */
        inline Frequency rateFrequency() const { return rateFreq_; };

        /**
         * @brief Returns the rate compounding.
         */
        inline Compounding rateCompounding() const { return rateComp_; };

        /**
         * @brief Adds a fixing to the history.
         * @param date The date of the fixing.
         * @param value The value of the fixing.
         */
        inline void addFixing(const Date& date, double value) { fixingHistory_[date] = value; };

        /**
         * @brief Returns the fixing for a given date.
         * @param date The date of the fixing.
         */
        double getFixing(const Date& date) const;

        std::unique_ptr<RateIndex> copy() const;

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
