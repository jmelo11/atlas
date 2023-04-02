#ifndef AA32C5FD_25FD_4E54_B19F_3D8E3EF02A47
#define AA32C5FD_25FD_4E54_B19F_3D8E3EF02A47

#include <atlas/models/model.hpp>

namespace Atlas {
    template <typename adouble>
    class StaticYieldModel : public Model<adouble> {
       public:
        StaticYieldModel(const MarketRequest& marketRequest, const InterestRate<adouble>& yield, const Date& refDate = Date())
        : Model<adouble>(marketRequest), yield_(yield), refDate_(refDate){};

        StaticYieldModel(const MarketRequest& marketRequest, const Date& refDate = Date()) : Model<adouble>(marketRequest), refDate_(refDate){};

        void simulate(const std::vector<Date>& evalDates, Scenario<adouble>& scenario) const override {
            for (const auto& evalDate : evalDates) {
                MarketData<adouble> marketData;
                marketData.allocate(this->marketRequest_);

                simulateDiscounts(evalDate, marketData);
                simulateForwards(evalDate, marketData);
                marketData.refDate = evalDate;
                scenario.push_back(marketData);
            }
        };

        void yield(const InterestRate<adouble>& y) { yield_ = y; };

        InterestRate<adouble> yield() const { return yield_; };

        MarketData<adouble> simulate() const {
            std::vector<Date> dates = {refDate_};
            Scenario<adouble> scenario;
            simulate(dates, scenario);
            return scenario.front();
        };

       private:
        void simulateDiscounts(const Date& refDate, MarketData<adouble>& md) const {
            for (auto& request : this->marketRequest_.dfs) {
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

        void simulateForwards(const Date& refDate, MarketData<adouble>& md) const {
            for (auto& request : this->marketRequest_.fwds) {
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

        InterestRate<adouble> yield_;
        Date refDate_;
    };
}  // namespace Atlas

#endif /* AA32C5FD_25FD_4E54_B19F_3D8E3EF02A47 */
