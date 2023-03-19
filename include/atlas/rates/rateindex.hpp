#ifndef A53B011D_D929_446B_92A0_CDFF70167D4D
#define A53B011D_D929_446B_92A0_CDFF70167D4D

#include <atlas/atlasconfig.hpp>
#include <map>

namespace Atlas {

    /***
     * @brief Configuration for a rate index.
     */
    class RateIndexConfiguration {
       public:
        RateIndexConfiguration() = default;

        RateIndexConfiguration(std::string name, Frequency fixingFreq, DayCounter dayCounter = Actual360(), Frequency rateFreq = Frequency::Annual,
                               Compounding rateComp = Compounding::Simple)
        : name_(name), dayCounter_(dayCounter), fixingFreq_(fixingFreq), rateFreq_(rateFreq), rateComp_(rateComp){};

        std::string name() const { return name_; }

        DayCounter dayCounter() const { return dayCounter_; }

        Frequency fixingFrequency() const { return fixingFreq_; }

        Frequency rateFrequency() const { return rateFreq_; }

        Compounding rateCompounding() const { return rateComp_; }

       private:
        std::string name_      = "undefined";
        DayCounter dayCounter_ = DayCounter();
        Frequency fixingFreq_  = Frequency::NoFrequency;
        Frequency rateFreq_    = Frequency::NoFrequency;
        Compounding rateComp_  = Compounding::Simple;
    };

    /***
     * @brief A rate index.
     */
    class RateIndex {
       public:
        RateIndex() = default;

        RateIndex(RateIndexConfiguration config, std::map<Date, double> fixingHistory = std::map<Date, double>())
        : config_(config), fixingHistory_(fixingHistory){};

        RateIndexConfiguration config() const { return config_; }

        void addFixing(const Date& date, double value) { fixingHistory_[date] = value; }  // how does it work with simulations?

        double getFixing(const Date& date) const {
            auto it = fixingHistory_.find(date);
            if (it != fixingHistory_.end()) { return it->second; }
            {
                std::string msg = "No fixing found for date";
                throw std::runtime_error(msg);
            }
        }

       private:
        RateIndexConfiguration config_;
        std::map<Date, double> fixingHistory_;
    };

}  // namespace Atlas

#endif /* A53B011D_D929_446B_92A0_CDFF70167D4D */
