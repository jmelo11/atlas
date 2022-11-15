#include <atlas/models/staticcurvemodel.hpp>

namespace Atlas {

    StaticCurveModel::StaticCurveModel(
        const MarketRequest& marketRequest, const CurveMap& discountCurves,
        const CurveMap& forecastCurves,
        DoubleMap<std::string, QuantLib::Date, double> historicalData)
    : Model(marketRequest), discountCurves_(discountCurves), forecastCurves_(forecastCurves) {
        if (checkRefDates()) {
            if (!discountCurves_.empty()) { refDate_ = discountCurves_.begin()->second->refDate(); }
            if (!forecastCurves_.empty()) { refDate_ = forecastCurves_.begin()->second->refDate(); }

        } else {
            throw std::runtime_error("All curves reference date must be equal.");
        }
    };

    void StaticCurveModel::simulate(const std::vector<QuantLib::Date>& evalDates,
                                    Scenario& scenario) const {
        for (const auto& evalDate : evalDates) {
            MarketData marketData;
            marketData.allocate(marketRequest_);

            simulateDiscounts(marketData);
            if (!forecastCurves_.empty()) { simulateForwards(marketData); }
            marketData.refDate = evalDate;
            scenario.push_back(marketData);
        }
    }

    void StaticCurveModel::simulateDiscounts(MarketData& md) const {
        for (auto& request : marketRequest_.dfs) {
            const auto& curve = discountCurves_.at(request.discountCurve_);
            const auto& date  = request.date_;
            double df;
            if (curve->refDate() < date) {
                df = curve->df(date);
            } else if (curve->refDate() == date) {
                df = 1;
            } else {
                df = 0;
            }
            md.dfs.push_back(df);
        }
    }

    void StaticCurveModel::simulateForwards(MarketData& md) const {
        for (auto& request : marketRequest_.fwds) {
            const auto& curve     = forecastCurves_.at(request.curve_);
            const auto& startDate = request.startDate_;
            const auto& endDate   = request.endDate_;
            double fwd;
            if (curve->refDate() <= startDate) {
                fwd = curve->forwardRate(startDate, endDate);
            } else {
                fwd = historicalData_.at(request.curve_).at(startDate);
            }
            md.fwds.push_back(fwd);
        }
    };

    bool StaticCurveModel::checkRefDates() {
        if (!discountCurves_.empty()) {
            auto it          = discountCurves_.begin();
            QuantLib::Date a = it->second->refDate();
            for (auto& [k, v] : discountCurves_) {
                QuantLib::Date b = v->refDate();
                if (a != b) { return false; }
            }
        }
        if (!forecastCurves_.empty()) {
            auto it          = forecastCurves_.begin();
            QuantLib::Date a = it->second->refDate();
            for (auto& [k, v] : forecastCurves_) {
                QuantLib::Date b = v->refDate();
                if (a != b) { return false; }
            }
        }
        return true;
    }

}  // namespace Atlas