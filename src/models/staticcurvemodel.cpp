#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/rates/rateindex.hpp>

namespace Atlas {

    StaticCurveModel::StaticCurveModel(const MarketRequest& marketRequest) : Model(marketRequest), curveStore_(CurveContextStore::instance()){};

    void StaticCurveModel::simulate(const std::vector<QuantLib::Date>& evalDates, Scenario& scenario) const {
        for (const auto& evalDate : evalDates) {
            MarketData marketData;
            marketData.allocate(marketRequest_);

            simulateDiscounts(marketData);
            simulateForwards(marketData);

            marketData.refDate = evalDate;
            scenario.push_back(marketData);
        }
    }

    MarketData StaticCurveModel::simulate(const Date& refDate) const {
        std::vector<QuantLib::Date> dates = {refDate};
        Scenario scenario;
        simulate(dates, scenario);
        return scenario.front();
    }

    void StaticCurveModel::simulateDiscounts(MarketData& md) const {
        for (auto& request : marketRequest_.dfs) {
            size_t idx                                = request.curve_;
            const Date& date                          = request.date_;
            const CurveContext& curveContext          = curveStore_.at(idx);
            const QuantLib::YieldTermStructure& curve = curveContext.curve();

            double df;
            if (curve.referenceDate() < date) {
                df = curve.discount(date);
            } else if (curve.referenceDate() == date) {
                df = 1;
            } else {
                df = 0;
            }
            md.dfs.push_back(df);
        }
    }

    void StaticCurveModel::simulateForwards(MarketData& md) const {
        for (auto& request : marketRequest_.fwds) {
            size_t idx            = request.curve_;
            const Date& startDate = request.startDate_;
            const Date& endDate   = request.endDate_;

            const auto& curveContext = curveStore_.at(idx);
            const auto& curve        = curveContext.curve();
            const auto& index        = curveContext.index();

            double fwd;
            if (curve.referenceDate() <= startDate) {
                fwd = curve.forwardRate(startDate, endDate, index.dayCounter(), index.rateCompounding(), index.rateFrequency()).rate();
            } else {
                fwd = index.getFixing(startDate);
            }
            md.fwds.push_back(fwd);
        }
    };

}  // namespace Atlas