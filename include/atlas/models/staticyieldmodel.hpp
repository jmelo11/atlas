#ifndef AA32C5FD_25FD_4E54_B19F_3D8E3EF02A47
#define AA32C5FD_25FD_4E54_B19F_3D8E3EF02A47

#include <atlas/models/model.hpp>

namespace Atlas {

    /**
     * @brief Static yield model. Generates scenarios based on a single yield. Mainly for educational purposes.
     *
     * @tparam adouble
     */
    template <typename adouble>
    class StaticYieldModel : public Model<adouble> {
       public:
        StaticYieldModel(const MarketRequest& marketRequest, const InterestRate<adouble>& yield) : Model<adouble>(marketRequest), yield_(yield){};

        StaticYieldModel(const MarketRequest& marketRequest) : Model<adouble>(marketRequest){};

        void simulate(const std::vector<Date>& evalDates, Scenario<adouble>& scenario) const override {
            for (const auto& evalDate : evalDates) {
                MarketData<adouble> marketData;
                marketData.allocate(this->marketRequest_);

                simulateDiscounts(evalDate, marketData);
                simulateForwards(evalDate, marketData);
                simulateSpots(evalDate, marketData);
                marketData.refDate = evalDate;
                scenario.push_back(marketData);
            }
        };

        void yield(const InterestRate<adouble>& y) { yield_ = y; };

        InterestRate<adouble> yield() const { return yield_; };

        MarketData<adouble> simulate(const Date& refDate) const {
            std::vector<Date> dates = {refDate};
            Scenario<adouble> scenario;
            simulate(dates, scenario);
            return scenario.front();
        };

       protected:
        void simulateDiscounts(const Date& refDate, MarketData<adouble>& md) const override {
            for (const auto& request : this->marketRequest_.dfs) {
                const auto& date = request.date_;
                adouble df;
                if (refDate < date) {
                    df = yield_.discountFactor(refDate, date);
                } else if (refDate == date) {
                    df = 1;
                } else {
                    df = 0;
                }
                md.dfs.push_back(df);
            }
        };

        void simulateForwards(const Date& refDate, MarketData<adouble>& md) const override {
            for (const auto& request : this->marketRequest_.fwds) {
                const auto& startDate = request.startDate_;
                const auto& endDate   = request.endDate_;
                adouble fwd;

                if (endDate < refDate) {
                    fwd = 0;
                } else {
                    fwd = yield_
                              .impliedRate(yield_.compoundFactor(startDate, endDate), request.dayCounter_, request.compounding_, request.frequency_,
                                           startDate, endDate)
                              .rate();
                }
                md.fwds.push_back(fwd);
            }
        };

        /**
         * @brief Simulate the spot rates. If the date is not provided, the spot rate is used, otherwise the fx function is used
         * to forecast the spot rate.
         *
         * @param evalDate
         * @param md
         */
        void simulateSpots(const Date& evalDate, MarketData<adouble>& md) const override {
            if (this->marketRequest_.spots.size() > 0) throw std::invalid_argument("StaticYieldModel: spots not supported.");
        }

        InterestRate<adouble> yield_;
    };
}  // namespace Atlas

#endif /* AA32C5FD_25FD_4E54_B19F_3D8E3EF02A47 */
