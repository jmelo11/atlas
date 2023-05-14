#ifndef EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406
#define EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/derivatives/fixfloatswap.hpp>
#include <atlas/instruments/derivatives/fxforward.hpp>
#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>
#include <set>
#include <unordered_map>

namespace Atlas {

    /**
     * @brief A class for indexing the cashflows of an instrument.
     *
     * @tparam adouble
     */
    template <typename adouble>
    class Indexer : public Visitor<adouble> {
       public:
        Indexer(){};

        /**
         * @brief Indexes the cashflows of a fixed rate instrument.
         *
         * @param inst
         */
        void visit(FixedRateInstrument<adouble>& inst) override {
            indexCashflow(inst.disbursement());
            indexExchangeRate(inst.disbursement(), false);
            auto& leg         = inst.leg();
            auto& coupons     = leg.coupons();
            auto& redemptions = leg.redemptions();

            for (auto& coupon : coupons) { indexCashflow(coupon); }
            for (auto& redemption : redemptions) { indexCashflow(redemption); }
        };

        /**
         * @brief Indexes the cashflows of a floating rate instrument.
         *
         * @param inst
         */
        void visit(FloatingRateInstrument<adouble>& inst) override {
            indexCashflow(inst.disbursement());
            indexExchangeRate(inst.disbursement(), false);
            auto& leg         = inst.leg();
            auto& coupons     = leg.coupons();
            auto& redemptions = leg.redemptions();

            for (auto& coupon : coupons) { indexCashflow(coupon); }
            for (auto& redemption : redemptions) { indexCashflow(redemption); }
        };

        /**
         * @brief Indexes the cashflows of an FX forward.
         *
         * @param inst
         */
        void visit(FxForward<adouble>& inst) override {
            auto& ccy1Cashflow = inst.leg().redemptions()[0];
            auto& ccy2Cashlfow = inst.leg().redemptions()[1];
            MarketRequest::ExchangeRate fwdPrice(ccy1Cashflow.currencyCode(), ccy2Cashlfow.currencyCode(), inst.endDate());
            MarketRequest::ExchangeRate spotPrice(ccy1Cashflow.currencyCode(), 0, Date());

            if (fxPricesMap_.find(fwdPrice) == fxPricesMap_.end()) {
                std::lock_guard<std::mutex> lock(mtx_);
                fxPricesVector_.push_back(fwdPrice);
                fxPricesMap_[fwdPrice] = fxPricesVector_.size() - 1;
            }
            if (fxPricesMap_.find(spotPrice) == fxPricesMap_.end()) {
                std::lock_guard<std::mutex> lock(mtx_);
                fxPricesVector_.push_back(spotPrice);
                fxPricesMap_[spotPrice] = fxPricesVector_.size() - 1;
            }
            ccy1Cashflow.fxIdx(fxPricesMap_.at(fwdPrice));
            ccy2Cashlfow.fxIdx(fxPricesMap_.at(spotPrice));
            indexCashflow(ccy1Cashflow);
        };

        /**
         * @brief Indexes the cashflows of a fixed float swap.
         *
         * @param inst
         */
        void visit(FixFloatSwap<adouble>& inst) override {
            auto& firstLeg  = inst.firstLeg();
            auto& secondLeg = inst.secondLeg();

            for (auto& coupon : firstLeg.coupons()) { indexCashflow(coupon); }
            for (auto& redemption : firstLeg.redemptions()) { indexCashflow(redemption); }

            for (auto& coupon : secondLeg.coupons()) { indexCashflow(coupon); }
            for (auto& redemption : secondLeg.redemptions()) { indexCashflow(redemption); }

            indexExchangeRate(firstLeg.coupons()[0], false);
        }

        /**
         * @brief Set the Request object
         *
         * @param request
         */
        void setRequest(MarketRequest& request) {
            request.dfs    = dfsVector_;
            request.fwds   = fwdsVector_;
            request.prices = pricesVector_;
            request.fxs    = fxPricesVector_;
        };

        /**
         * @brief Get the Request object
         *
         * @return MarketRequest
         */
        MarketRequest request() {
            MarketRequest request;
            setRequest(request);
            return request;
        };

        /**
         * @brief Clears the indexer.
         *
         */
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

            std::lock_guard<std::mutex> lock(mtx_);
            evaluationDates_.insert(paymentDate);

            MarketRequest::DiscountFactor df(curveIdx, paymentDate);

            // If the discount factor has not been indexed, add it to the vector
            if (dfsMap_.find(df) == dfsMap_.end()) {
                dfsVector_.push_back(df);
                dfsMap_[df] = dfsVector_.size() - 1;
            }
            cashflow.dfIdx(dfsMap_[df]);

            // fwd rate
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

            // fx
            if (cashflow.applyCcy()) { indexExchangeRate(cashflow, true); }
        };

        /**
         * @brief Indexes an exchange rate.
         * 
         * @param cashflow 
         * @param atPaymentDate if true, the exchange rate is indexed at the payment date of the cashflow, otherwise is set to Date() (evaluation date).
         */
        void indexExchangeRate(Cashflow<adouble>& cashflow, bool atPaymentDate = false) {
            Date fxDate;
            if (atPaymentDate) {
                fxDate = cashflow.paymentDate();
            } else {
                fxDate = Date();
            }
            MarketRequest::ExchangeRate fx(cashflow.currencyCode(), 0, fxDate);  // cashflow ccy to local ccy

            std::lock_guard<std::mutex> lock(mtx_);
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
        std::mutex mtx_;
    };
}  // namespace Atlas

#endif /* EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406 */
