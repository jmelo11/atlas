#ifndef CFD5072A_D056_4851_AD95_2569DB4157DD
#define CFD5072A_D056_4851_AD95_2569DB4157DD

#include <atlas/atlasconfig.hpp>
#include <map>

namespace Atlas {

    /**
     * @class Index
     * @brief Index interface
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class Index {
       public:
        Index() = default;

        /**
         * @brief Construct a new Index object
         *
         * @param fixingFreq
         */
        Index(Frequency fixingFreq) : fixingFreq_(fixingFreq){};

        /**
         * @brief Destroy the Index object
         *
         */
        virtual ~Index() = default;

        /**
         * @brief Adds a index fixing value
         *
         * @param date
         * @param fixing
         */
        inline void addHistoricalFixing(const Date& date, double fixing) { historicalFixings_[date] = fixing; };

        /**
         * @brief Sets the fixing map
         *
         * @param fixings
         */
        inline void addHistoricalFixings(const std::map<Date, double>& fixings) {
            for (const auto& fixing : fixings) { addHistoricalFixing(fixing.first, fixing.second); }
        };

        /**
         * @brief Gets the fixing relative to the given date
         *
         * @param date
         * @return adouble
         */
        virtual adouble fixing(const Date& date) const { return historicalFixings_.at(date); };

        /**
         * @brief Returns the fixing frequency of the index
         *
         * @return Frequency
         */
        inline Frequency fixingFrequency() const { return fixingFreq_; };

        /**
         * @brief Checks if the fixing map is empty
         *
         * @return true
         * @return false
         */
        inline bool empty() const { return historicalFixings_.empty(); };

       protected:
        std::map<Date, double> historicalFixings_;
        Frequency fixingFreq_ = Frequency::NoFrequency;
    };
}  // namespace Atlas

#endif /* CFD5072A_D056_4851_AD95_2569DB4157DD */
