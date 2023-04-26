#ifndef EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406
#define EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/derivatives/fxforward.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>
#include <set>
#include <unordered_map>

namespace Atlas {

    template <typename adouble>
    class Indexer : public Visitor<adouble> {
       public:
        Indexer(){};

        void visit(FixedRateInstrument<adouble>& inst) override {
            indexCashflow(inst.disbursement());
            indexExchangeRate(inst.disbursement(), false);
            auto& leg         = inst.leg();
            auto& coupons     = leg.coupons();
            auto& redemptions = leg.redemptions();

            for (auto& coupon : coupons) { indexCashflow(coupon); }
            for (auto& redemption : redemptions) { indexCashflow(redemption); }
        };

        void visit(FloatingRateInstrument<adouble>& inst) override {
            indexCashflow(inst.disbursement());
            indexExchangeRate(inst.disbursement(), false);
            auto& leg         = inst.leg();
            auto& coupons     = leg.coupons();
            auto& redemptions = leg.redemptions();

            for (auto& coupon : coupons) { indexCashflow(coupon); }
            for (auto& redemption : redemptions) { indexCashflow(redemption); }
        };

        void visit(FxForward<adouble>& inst) override{
            // npv = (strikePrice (823 CLP/USD) -fwdPrice (835 CLP/USD))*notional (100_000 USD) * dfLocal (CLP) * localCcy (CLP/CLP)-> OK
            // npv = (strikePrice (1.1 USD/EUR) -fwdPrice (1.02 USD/EUR))*notional (100_000 EUR) *  dfLocal (CLP) * localCcy (CLP/USD) -> OK
            // auto& strikeCashflow = inst.leg().redemptions()[0];
            // auto& mktCashflow    = inst.leg().redemptions()[1];
            // size_t ccy1          = strikeCashflow.currencyContextIdx();
            // size_t ccy2          = mktCashflow.currencyContextIdx();
            // MarketRequest::FxPrice fwdPrice(ccy1, ccy2, inst.endDate());
            // MarketRequest::FxPrice spotPrice(ccy1, 0, Date());
            // if (fxPricesMap_.find(fwdPrice) == fxPricesMap_.end()) {
            //     fxPricesVector_.push_back(fwdPrice);
            //     fxPricesMap_[fwdPrice] = fxPricesVector_.size() - 1;
            // }
            // if (fxPricesMap_.find(spotPrice) == fxPricesMap_.end()) {
            //     fxPricesVector_.push_back(spotPrice);
            //     fxPricesMap_[spotPrice] = fxPricesVector_.size() - 1;
            // }
            // strikeCashflow.fxIdx(fxPricesMap_[fwdPrice]);
            // mktCashflow.fxIdx(fxPricesMap_[spotPrice]);
            // indexCashflow(strikeCashflow);
        };

        void setRequest(MarketRequest& request) {
            request.dfs    = dfsVector_;
            request.fwds   = fwdsVector_;
            request.prices = pricesVector_;
            request.fxs    = fxPricesVector_;
        };

        MarketRequest request() {
            MarketRequest request;
            setRequest(request);
            return request;
        };

        void clear() {
            dfsMap_.clear();
            dfsVector_.clear();
            fwdsMap_.clear();
            fwdsVector_.clear();
            pricesMap_.clear();
            pricesVector_.clear();
        };

       private:
        /**
         * @brief Indexes a cashflow
         *
         * @tparam Flow
         * @param cashflow
         */
        template <typename Flow>
        void indexCashflow(Flow& cashflow) {
            if (!cashflow.hasDiscountContext()) { throw std::runtime_error("Cashflow does not have a discount curve context."); }
            size_t curveIdx         = cashflow.discountContextIdx();
            const Date& paymentDate = cashflow.paymentDate();
            evaluationDates_.insert(paymentDate);

            MarketRequest::DiscountFactor df(curveIdx, paymentDate);

            // If the discount factor has not been indexed, add it to the vector
            if (dfsMap_.find(df) == dfsMap_.end()) {
                dfsVector_.push_back(df);
                dfsMap_[df] = dfsVector_.size() - 1;
            }
            cashflow.dfIdx(dfsMap_[df]);

            //fwd rate
            if constexpr (std::is_same_v<Flow, FloatingRateCoupon<adouble>>) {
                if (!cashflow.hasForecastContext()) { throw std::runtime_error("Cashflow does not have a forecast curve context."); }
                size_t curveIdx = cashflow.forecastContextIdx();
                MarketRequest::ForwardRate fwd(cashflow.startDate(), cashflow.endDate(), curveIdx);
                if (fwdsMap_.find(fwd) == fwdsMap_.end()) {
                    fwdsVector_.push_back(fwd);
                    fwdsMap_[fwd] = fwdsVector_.size() - 1;
                }
                cashflow.fwdIdx(fwdsMap_[fwd]);
            }

            //fx
            if (cashflow.applyCcy()) { indexExchangeRate(cashflow, true); }
        };

        void indexExchangeRate(Cashflow<adouble>& cashflow, bool atPaymentDate = false) {
            Date fxDate;
            if (atPaymentDate) {
                fxDate = cashflow.paymentDate();
            } else {
                fxDate = Date();
            }
            MarketRequest::ExchangeRate fx(cashflow.currencyCode(), 0, fxDate);  // cashflow ccy to local ccy
            if (fxPricesMap_.find(fx) == fxPricesMap_.end()) {
                fxPricesVector_.push_back(fx);
                fxPricesMap_[fx] = fxPricesVector_.size() - 1;
            }
            cashflow.fxIdx(fxPricesMap_[fx]);
        }

        std::unordered_map<MarketRequest::ForwardRate, size_t> fwdsMap_;
        std::vector<MarketRequest::ForwardRate> fwdsVector_;

        std::unordered_map<MarketRequest::DiscountFactor, size_t> dfsMap_;
        std::vector<MarketRequest::DiscountFactor> dfsVector_;

        std::vector<MarketRequest::Price> pricesVector_;
        std::unordered_map<MarketRequest::Price, size_t> pricesMap_;

        std::vector<MarketRequest::ExchangeRate> fxPricesVector_;
        std::unordered_map<MarketRequest::ExchangeRate, size_t> fxPricesMap_;

        std::set<Date> evaluationDates_;
    };
}  // namespace Atlas

#endif /* EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406 */
