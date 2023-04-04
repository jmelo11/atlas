/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288
#define DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288

#include <atlas/models/model.hpp>
#include <atlas/rates/curvecontextstore.hpp>
#include <map>

namespace Atlas {

    /**
     * @brief Static curve model
     * 
     * @tparam adouble 
     */
    template <typename adouble>
    class StaticCurveModel : public Model<adouble> {
       public:
        StaticCurveModel(const MarketRequest& marketRequest) : Model<adouble>(marketRequest), curveStore_(CurveContextStore::instance()){};

        /**
         * @brief Set the Request object
         * 
         * @param request 
         */
        void setRequest(const MarketRequest& request) { this->marketRequest_ = request; };

        /**
         * @brief Simulate the market data for a given scenario
         * 
         * @param evalDates 
         * @param scenario 
         */
        void simulate(const std::vector<Date>& evalDates, Scenario<adouble>& scenario) const override {
            for (const auto& evalDate : evalDates) {
                MarketData<adouble> marketData;
                marketData.allocate(this->marketRequest_);
                simulateDiscounts(marketData);
                simulateForwards(marketData);

                marketData.refDate = evalDate;
                scenario.push_back(marketData);
            }
        };

        /**
         * @brief simulate base on one date
         * 
         * @param refDate 
         * @return MarketData<adouble> 
         */
        MarketData<adouble> simulate(const Date& refDate) const {
            std::vector<QuantLib::Date> dates = {refDate};
            Scenario<adouble> scenario;
            simulate(dates, scenario);
            return scenario.front();
        };

       private:
        void simulateDiscounts(MarketData<adouble>& md) const {
            for (auto& request : this->marketRequest_.dfs) {
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
        };

        void simulateForwards(MarketData<adouble>& md) const {
            for (auto& request : this->marketRequest_.fwds) {
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

        const CurveContextStore& curveStore_;
    };
}  // namespace Atlas

#endif /* DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288 */
