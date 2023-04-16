/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288
#define DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288

#include <atlas/fundation/marketstore.hpp>
#include <atlas/models/model.hpp>
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
        StaticCurveModel(const MarketRequest& marketRequest, const MarketStore<adouble>& marketStore)
        : Model<adouble>(marketRequest), marketStore_(marketStore){};

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
                simulateDiscounts(evalDate, marketData);
                simulateForwards(evalDate, marketData);
                simulateSpots(evalDate, marketData);
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
        MarketData<adouble> simulate(const Date& evalDate) const {
            std::vector<QuantLib::Date> dates = {evalDate};
            Scenario<adouble> scenario;
            simulate(dates, scenario);
            return scenario.front();
        };

       protected:
        /**
         * @brief Simulate the discount factors for a given scenario. If the evaluation date is after the curve reference date, a "forward" discount
         * factor will be calculated.
         *
         * @param evalDate
         * @param md
         */
        void simulateDiscounts(const Date& evalDate, MarketData<adouble>& md) const override {
            for (size_t i = 0; i < this->marketRequest_.dfs.size(); ++i) {
                auto& request            = this->marketRequest_.dfs[i];
                size_t idx               = request.curve_;
                const Date& date         = request.date_;
                const auto& curveContext = marketStore_.curveContext(idx);
                const auto& curve        = curveContext.curve();

                if (curve.refDate() > evalDate) { throw std::invalid_argument("Curve reference date does not match the simulation reference date"); }

                adouble df;
                if (evalDate < date) {
                    adouble df0 = curve.discount(evalDate);
                    adouble df1 = curve.discount(date);  // shift valuation date? or raise exception?
                    df          = df1 / df0;
                } else if (evalDate == date) {
                    df = 1.0;
                } else {
                    df = 0.0;
                }
                md.dfs.push_back(df);
            }
        };

        /**
         * @brief Simulate the forward rates. Uses the curves to forecast each forward rate.
         *
         * @param evalDate
         * @param md
         */
        void simulateForwards(const Date& evalDate, MarketData<adouble>& md) const override {
            for (size_t i = 0; i < this->marketRequest_.fwds.size(); ++i) {
                auto& request         = this->marketRequest_.fwds[i];
                size_t idx            = request.curve_;
                const Date& startDate = request.startDate_;
                const Date& endDate   = request.endDate_;

                const auto& curveContext = marketStore_.curveContext(idx);
                const auto& curve        = curveContext.curve();
                const auto& index        = curveContext.index();

                adouble fwd;
                if (evalDate <= startDate) {
                    fwd = curve.forwardRate(startDate, endDate, index.dayCounter(), index.rateCompounding(), index.rateFrequency());
                } else {
                    fwd = index.getFixing(startDate);
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
            for (size_t i = 0; i < this->marketRequest_.spots.size(); ++i) {
                auto& request    = this->marketRequest_.spots[i];
                size_t idx       = request.ccy_;
                const auto& date = request.date_;
                adouble spot;
                if (marketStore_.currencyContext(idx).recepy() != nullptr) {
                    auto fx = marketStore_.currencyContext(idx).recepy();
                    spot    = fx(evalDate, date, marketStore_);
                } else {
                    spot = marketStore_.currencyContext(idx).value();
                }
                md.spots.push_back(spot);
            }
        }

        const MarketStore<adouble>& marketStore_;
    };
}  // namespace Atlas

#endif /* DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288 */
