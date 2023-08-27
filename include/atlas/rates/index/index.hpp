#ifndef CFD5072A_D056_4851_AD95_2569DB4157DD
#define CFD5072A_D056_4851_AD95_2569DB4157DD

#include <atlas/rates/index/ratedefinition.hpp>
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
       protected:
        class Strategy {
           public:
            Strategy()          = default;
            virtual ~Strategy() = default;
            Strategy(const Period& tenor, const RateDefinition& rateDef) : tenor_(tenor), rateDef_(rateDef){};
            inline adouble fixing(const Date& date) const { return historicalFixings_.at(date); };
            inline Period tenor() const { return tenor_; };
            inline bool empty() const { return historicalFixings_.empty(); };  // namespace Atlas
            inline void addHistoricalFixing(const Date& date, double fixing) { historicalFixings_[date] = fixing; };
            inline RateDefinition rateDefinition() const { return rateDef_; }
            inline const Date& refDate() const { return historicalFixings_.begin().first; }
            virtual std::unique_ptr<Strategy> clone() const                                                        = 0;
            virtual adouble forecastRate(const Date& fixingDate, const Date& startDate, const Date& endDate) const = 0;

            inline bool isValid() const { return tenor_ != Period(); };

           protected:
            std::map<Date, double> historicalFixings_;
            Period tenor_ = Period();
            RateDefinition rateDef_;
        };

       public:
        Index() = default;

        Index(std::unique_ptr<Strategy> strategy) : strategy_(std::move(strategy)){};

        Index(const Index& rhs) {
            if (rhs.strategy_ != nullptr) strategy_ = rhs.strategy_->clone();
        };

        Index operator=(const Index& index) {
            if (this != &index) strategy_ = std::make_unique<Strategy>(index.strategy_);
            return *this;
        };

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
        inline void addHistoricalFixing(const Date& date, double fixing) { strategy_->addHistoricalFixing(date, fixing); };

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
        inline virtual adouble fixing(const Date& date) const { return strategy_->fixing(date); };

        /**
         * @brief Returns the fixing frequency of the index
         *
         * @return Frequency
         */
        inline Period tenor() const { return strategy_->tenor(); };

        /**
         * @brief Checks if the fixing map is empty
         *
         * @return true
         * @return false
         */
        inline bool empty() const { strategy_->empty(); };

        inline RateDefinition rateDefinition() const { return strategy_->rateDefinition(); }

        inline adouble forecastRate(const Date& fixingDate, const Date& startDate, const Date& endDate) const {
            return strategy_->forecastRate(fixingDate, startDate, endDate);
        }

        inline bool isValid() const { return strategy_->isValid(); };

       protected:
        std::unique_ptr<Strategy> strategy_;
    };  // namespace Atlas
}  // namespace Atlas

#endif /* CFD5072A_D056_4851_AD95_2569DB4157DD */
