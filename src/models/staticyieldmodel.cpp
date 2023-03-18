#include <atlas/models/staticyieldmodel.hpp>

namespace Atlas {

    StaticYieldModel::StaticYieldModel(const MarketRequest& marketRequest, const InterestRate& yield, const Date& refDate)
    : Model(marketRequest), yield_(yield), refDate_(refDate) {}

    StaticYieldModel::StaticYieldModel(const MarketRequest& marketRequest, const Date& refDate) : Model(marketRequest), refDate_(refDate) {}

    void StaticYieldModel::simulate(const std::vector<Date>& evalDates, Scenario& scenario) const {
        for (const auto& evalDate : evalDates) {
            MarketData marketData;
            marketData.allocate(marketRequest_);

            simulateDiscounts(evalDate, marketData);
            simulateForwards(evalDate, marketData);
            marketData.refDate = evalDate;
            scenario.push_back(marketData);
        }
    }

    void StaticYieldModel::simulateDiscounts(const Date& refDate, MarketData& md) const {
        for (auto& request : marketRequest_.dfs) {
            const auto& date = request.date_;
            double df;
            if (refDate < date) {
                df = yield_.discountFactor(refDate, date);
            } else if (refDate == date) {
                df = 1;
            } else {
                df = 0;
            }
            md.dfs.push_back(df);
        }
    }

    void StaticYieldModel::simulateForwards(const Date& refDate, MarketData& md) const {
        for (auto& request : marketRequest_.fwds) {
            const auto& startDate = request.startDate_;
            const auto& endDate   = request.endDate_;
            double fwd;

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
    }

    MarketData StaticYieldModel::simulate() const {
        std::vector<Date> dates = {refDate_};
        Scenario scenario;
        simulate(dates, scenario);
        return scenario.front();
    }

}  // namespace Atlas