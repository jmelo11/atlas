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
     * @class SpotMarketDataModel
     * @brief SpotMarketDataModel is a model that uses a set of curves and spot currencies to calculate the market data. Uses current information to
     * generate the spot.
     *
     * @tparam adouble
     */
    template <typename adouble>
    class SpotMarketDataModel : public Model<adouble> {
       public:
        SpotMarketDataModel(const MarketRequest& marketRequest, const MarketStore<adouble>& marketStore)
        : Model<adouble>(marketRequest), marketStore_(marketStore){};

        /**
         * @brief Simulate the market data for a given scenario
         *
         * @param evalDates
         * @param scenario
         */
        MarketData<adouble> marketData(const Date& evalDate = Date()) const override {
            Date evalDate_;
            if (evalDate == Date()) {
                evalDate_ = marketStore_.refDate();
            } else {
                evalDate_ = evalDate;
            }
            MarketData<adouble> marketData;
            marketData.allocate(this->marketRequest_);
            marketData.refDate = evalDate_;
            simulateDiscounts(evalDate_, marketData);
            simulateForwardRates(evalDate_, marketData);
            simulateExchangeRates(evalDate_, marketData);
            return marketData;
        };

       protected:
        /**
         * @brief Simulate the discount factors for a given scenario. If the evaluation date is after the curve reference date, a "forward" discount
         * factor will be calculated.
         *
         * @param evalDate
         * @param md
         */
        void simulateDiscounts(const Date& evalDate, MarketData<adouble>& md) const {
            for (size_t i = 0; i < this->marketRequest_.dfs.size(); ++i) {
                const auto& request                      = this->marketRequest_.dfs.at(i);
                size_t idx                               = request.curve_;
                const Date& date                         = request.date_;
                const YieldTermStructure<adouble>& curve = marketStore_.curveManager().curveContext(idx).curve();

                adouble df;
                if (evalDate < date) {
                    df = curve.discount(date);
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
        void simulateForwardRates(const Date& evalDate, MarketData<adouble>& md) const {
            for (size_t i = 0; i < this->marketRequest_.fwds.size(); ++i) {
                const auto& request   = this->marketRequest_.fwds.at(i);
                size_t idx            = request.curve_;
                const Date& startDate = request.startDate_;
                const Date& endDate   = request.endDate_;

                const auto& curveManager                 = marketStore_.curveManager();
                const YieldTermStructure<adouble>& curve = curveManager.curveContext(idx).curve();
                const InterestRateIndex<adouble>& index  = curveManager.curveContext(idx).index();
                const auto& rateDef                      = index.rateDefinition();
                adouble fwd;
                if (evalDate <= startDate) {
                    fwd = curve.forwardRate(startDate, endDate, rateDef.dayCounter(), rateDef.compounding(), rateDef.frequency());
                } else {
                    fwd = index.fixing(startDate);
                }
                md.fwds.push_back(fwd);
            }
        };

        /**
         * @brief Simulate the spot rates. If the date is not provided, the spot rate is used, otherwise a forward rate is calculated using
         * the risk free curves associated with each currency.
         *
         * @param evalDate
         * @param md
         */
        void simulateExchangeRates(const Date& evalDate, MarketData<adouble>& md) const {
            for (size_t i = 0; i < this->marketRequest_.fxs.size(); ++i) {
                const auto& request = this->marketRequest_.fxs.at(i);
                int ccy1         = request.ccy1_;
                int ccy2         = request.ccy2_;
                const Date& date    = request.date_;

                adouble fx = marketStore_.fxManager().exchangeRate(ccy1, ccy2);
                if (date != Date()) {
                    const auto& curveManager                     = marketStore_.curveManager();
                    size_t ccy1RFC                               = curveManager.riskFreeCurveIdx(ccy1);
                    size_t ccy2RFC                               = curveManager.riskFreeCurveIdx(ccy2);
                    const YieldTermStructure<adouble>& ccy1Curve = curveManager.curveContext(ccy1RFC).curve();
                    const YieldTermStructure<adouble>& ccy2Curve = curveManager.curveContext(ccy2RFC).curve();
                    fx *= ccy2Curve.discount(date) / ccy1Curve.discount(date);
                }
                md.fxs.push_back(fx);
            }
        };

        const MarketStore<adouble>& marketStore_;
    };
}  // namespace Atlas

#endif /* DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288 */
