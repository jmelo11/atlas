#ifndef C49C3B38_A428_4D92_AB3E_8E88957A544A
#define C49C3B38_A428_4D92_AB3E_8E88957A544A

#include <atlas/atlasconfig.hpp>
#include <atlas/fundation/contextmanager.hpp>
#include <atlas/fundation/exchangeratemanager.hpp>
#include <atlas/rates/rateindex.hpp>
#include <atlas/rates/yieldtermstructure.hpp>

namespace Atlas {

    /**
     * @brief A class representing a set of market information and market-related objects like curves, currencies and so on.
     * 
     * @tparam adouble 
     */
    template <typename adouble>
    class MarketStore {
       public:
        MarketStore(const Date& refDate, Currency localCcy = USD()) : refDate_(refDate), localCcy_(localCcy) {
            curveManager_     = std::make_unique<ContextManager<YieldTermStructure<adouble>>>(refDate_);
            rateIndexManager_ = std::make_unique<ContextManager<RateIndex<adouble>>>(refDate_);
            fxManager_        = std::make_unique<ExchangeRateManager<adouble>>(refDate_);
        };

        /**
         * @brief add a curve context to the store
         *
         * @param name name of the curve
         * @param curve
         * @param index
         */
        void addCurve(const std::string& name, const YieldTermStructure<adouble>& curve, const RateIndex<adouble>& index, Currency riskFreeCcy = Currency()) {
            if (curveManager_->hasContext(name)) {
                throw std::invalid_argument("A curve context with the given name already exists.");
            } else {
                curveManager_->createContext(name, curve);
                rateIndexManager_->createContext(name, index);
            }
            if (riskFreeCcy != Currency()) {
                riskFreeCurveIdx(riskFreeCcy, name);
            }
        };

        /**
         * @brief add an exchange rate to the store
         *
         * @param weak
         * @param strong
         * @param rate
         */
        void addExchangeRate(const Currency& weak, const Currency& strong, adouble rate) {
            if (fxManager_->hasExchangeRate(weak, strong)) {
                throw std::invalid_argument("An exchange rate with the given currencies already exists.");
            } else {
                fxManager_->addExchangeRate(weak, strong, rate);
            }
        };

        /**
         * @brief gets the curve context with the given index
         *
         * @param idx
         * @return const Context<YieldTermStructure<adouble>>&
         */
        const Context<YieldTermStructure<adouble>>& curveContext(size_t idx) const { return curveManager_->getContext(idx); };

        /**
         * @brief gets the curve context with the given name
         *
         * @param name
         * @return const Context<YieldTermStructure<adouble>>&
         */
        const Context<YieldTermStructure<adouble>>& curveContext(const std::string& name) const { return curveManager_->getContext(name); };

        /**
         * @brief gets the rate index context with the given name
         *
         * @param name
         * @return const Context<RateIndex<adouble>>&
         */
        const Context<RateIndex<adouble>>& rateIndexContext(const std::string& name) const { return rateIndexManager_->getContext(name); };

        /**
         * @brief gets the rate index context with the given index
         *
         * @param idx
         * @return const Context<RateIndex<adouble>>&
         */
        const Context<RateIndex<adouble>>& rateIndexContext(size_t idx) const { return rateIndexManager_->getContext(idx); };

        /**
         * @brief calculates the exchange rate between two currencies
         *
         * @param weak
         * @param strong
         * @return adouble
         */
        adouble exchange(const Currency& weak, const Currency& strong) const { return exchange(weak.numericCode(), strong.numericCode()); };

        /**
         * @brief calculates the exchange rate between two currencies
         *
         * @param weak
         * @param strong
         * @return adouble
         */
        adouble exchange(size_t weak, size_t strong) const { return fxManager_->exchange(translateCcyCode(weak), translateCcyCode(strong)); };

        /**
         * @brief return the currency associated risk free curve
         *
         * @param ccy
         * @return size_t
         */
        size_t riskFreeCurveIdx(const Currency& ccy) const { return riskFreeCurveIdx(ccy.numericCode()); };

        /**
         * @brief return the currency associated risk free curve
         * 
         * @param ccyCode 
         * @return size_t 
         */
        size_t riskFreeCurveIdx(size_t ccyCode) const { return fxManager_->riskFreeCurveIdx(translateCcyCode(ccyCode)); };

        /**
         * @brief Sets the currency associated risk free curve
         * 
         * @param ccy 
         * @param name 
         */
        void riskFreeCurveIdx(const Currency& ccy, const std::string& name) {
            auto& context = curveManager_->getContext(name);
            fxManager_->riskFreeCurveIdx(ccy, context);
        };

        /**
         * @brief gets the currency set as local
         *
         * @return const Currency&
         */
        const Currency& localCurrency() const { return localCcy_; };

        /**
         * @brief sets the currency as local
         *
         * @param ccy
         */
        void localCurrency(const Currency& ccy) { localCcy_ = ccy; };
        /**
         * @brief clone the market store
         *
         * @return MarketStore<adouble>
         */
        MarketStore<adouble> clone() const {
            MarketStore<adouble> store(refDate_, localCcy_);
            store.curveManager_     = curveManager_->clone();
            store.rateIndexManager_ = rateIndexManager_->clone();
            store.fxManager_        = fxManager_->clone();
            return store;
        };

        /**
         * @brief clone the market store from another store
         *
         * @param store
         */
        void cloneFromStore(const MarketStore<adouble>& store) {
            refDate_          = store.refDate_;
            localCcy_         = store.localCcy_;
            curveManager_     = store.curveManager_->clone();
            rateIndexManager_ = store.rateIndexManager_->clone();
            fxManager_        = store.fxManager_->clone();
        };

        inline Date refDate() const { return refDate_; };

       private:
        inline size_t translateCcyCode(size_t code) const {
            if (code == 0) {
                return localCcy_.numericCode();
            } else {
                return code;
            }
        }

        Date refDate_;
        Currency localCcy_;

        std::unique_ptr<ContextManager<YieldTermStructure<adouble>>> curveManager_;
        std::unique_ptr<ContextManager<RateIndex<adouble>>> rateIndexManager_;
        std::unique_ptr<ExchangeRateManager<adouble>> fxManager_;
    };
}  // namespace Atlas

#endif /* C49C3B38_A428_4D92_AB3E_8E88957A544A */
