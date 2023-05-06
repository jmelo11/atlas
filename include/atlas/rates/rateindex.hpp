#ifndef A53B011D_D929_446B_92A0_CDFF70167D4D
#define A53B011D_D929_446B_92A0_CDFF70167D4D

#include <atlas/atlasconfig.hpp>
#include <map>

namespace Atlas {

    /**
     * @brief A rate index.
     */
    template <typename adouble>
    class RateIndex {
       public:
        RateIndex() = default;

        /**
         * @brief Constructs a rate index.
         * @param name The name of the rate index.
         * @param fixingFreq The fixing frequency.
         * @param dayCounter The day counter.
         * @param rateFreq The rate frequency.
         * @param rateComp The rate compounding.
         */
        RateIndex(const Date& refDate, Frequency fixingFreq, DayCounter dayCounter = Actual360(), Frequency rateFreq = Frequency::Annual,
                  Compounding rateComp = Compounding::Simple)
        : refDate_(refDate), dayCounter_(dayCounter), fixingFreq_(fixingFreq), rateFreq_(rateFreq), rateComp_(rateComp){};

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
        adouble getFixing(const Date& date) const {
            if (date >= refDate_) {
                auto it = simFixings_.find(date);
                if (it != simFixings_.end()) { return it->second; }
                {
                    // ql date to string
                    std::string d   = std::to_string(date.year()) + "-" + std::to_string(date.month()) + "-" + std::to_string(date.dayOfMonth());
                    std::string msg = "No fixing found for date " + d;

                    throw std::runtime_error(msg);
                }
            } else {
                auto it = fixingHistory_.find(date);
                if (it != fixingHistory_.end()) { return it->second; }
                {
                    std::string d   = std::to_string(date.year()) + "-" + std::to_string(date.month()) + "-" + std::to_string(date.dayOfMonth());
                    std::string msg = "No fixing found for date " + d;
                    throw std::runtime_error(msg);
                }
            }
        };

        /**
         * @brief Clones the rate index.
         * @return std::unique_ptr<RateIndex>
         */
        std::unique_ptr<RateIndex> clone() const {
            auto copy            = std::make_unique<RateIndex>(refDate_, fixingFreq_, dayCounter_, rateFreq_, rateComp_);
            copy->fixingHistory_ = fixingHistory_;
            copy->simFixings_    = simFixings_;
            return copy;
        };

        /**
         * @brief Adds a simulated fixing.
         *
         * @param date
         * @param value
         */
        inline void addSimFixing(const Date& date, double value) { simFixings_[date] = value; };

        /**
         * @brief Clears the simulated fixing for a given date.
         *
         * @param date
         * @return void
         */
        inline void clearSimFixings() { simFixings_.clear(); };

        Date refDate() const { return refDate_; };

       private:
        Date refDate_;
        DayCounter dayCounter_ = DayCounter();
        Frequency fixingFreq_  = Frequency::NoFrequency;
        Frequency rateFreq_    = Frequency::NoFrequency;
        Compounding rateComp_  = Compounding::Simple;
        std::map<Date, double> fixingHistory_;
        mutable std::map<Date, adouble> simFixings_;  // adouble
    };

}  // namespace Atlas

#endif /* A53B011D_D929_446B_92A0_CDFF70167D4D */
