#ifndef C49C3B38_A428_4D92_AB3E_8E88957A544A
#define C49C3B38_A428_4D92_AB3E_8E88957A544A

#include <atlas/atlasconfig.hpp>
#include <atlas/fundation/contextmanager.hpp>
#include <atlas/fundation/currencycontext.hpp>
#include <atlas/fundation/exchangeratemanager.hpp>
#include <atlas/rates/curvecontext.hpp>

namespace Atlas {

    /**
     * @class MarketStore
     * @brief Class with thread local storage that stores all the curve contexts.
     */
    template <typename adouble>
    class MarketStore {
       public:
        /**
         * @brief Returns the instance of the store.
         * @param refDate The reference date.
         * @param localCcy The local currency. If not specified, the default currency code will be 0.
         */
        MarketStore(const Date& refDate = Date(), Currency localCcy = Currency()) : refDate_(refDate), localCcy_(localCcy) {
            // Create a default curve context for the local currency.
            adouble ccyValue = 1.0;
            createCurrencyContext("LocalCurrency", localCcy, ccyValue);
        }

        /**
         * @brief Creates a copy of curve context and adds it to the store.
         * @param contextName The name of the context.
         * @param curve The yield term structure.
         * @param index The rate index used as the underlying interest rate.
         * @param ccy The currency of the context. If not specified, the default currency code will be 0.
         */
        void createCurveContext(const std::string& contextName, const YieldTermStructure<adouble>& curve, const RateIndex& index) {
            if (curveToIdx_.count(contextName) == 0) {
                auto curveClone = curve.clone();
                auto indexClone = index.clone();
                CurveContext<adouble> context(curveClone, indexClone, curveContexts_.size());
                curveContexts_.push_back(std::move(context));
                curveToIdx_[contextName] = curveContexts_.size() - 1;
            } else {
                throw std::invalid_argument("A curve context with the given name already exists.");
            }
        };

        /**
         * @brief Create a Currency object
         *
         * @param currency
         * @param value
         * @param ccyRecepy
         */
        void createCurrencyContext(const std::string& contextName, const Currency& currency, adouble value, CcyRecepy<adouble> ccyRecepy = nullptr) {
            if (curveToIdx_.count(contextName) == 0) {
                CurrencyContext<adouble> context(currency, value, currencyContexts_.size(), ccyRecepy);
                currencyContexts_.push_back(context);
                curveToIdx_[contextName] = currencyContexts_.size() - 1;
            } else {
                throw std::invalid_argument("A currency context with the given name already exists.");
            }
        };

        /**
         * @brief Returns the thread local version of the curve context with the given name.
         * @param contextName The name of the context.
         * @return The curve context.
         */
        const CurveContext<adouble>& curveContext(const std::string& contextName) const {
            if (curveToIdx_.find(contextName) != curveToIdx_.end()) {
                size_t pos = curveToIdx_.at(contextName);
                return curveContexts_.at(pos);
            } else {
                std::string msg = "No curve context found with the given name: " + contextName;
                throw std::out_of_range(msg);
            }
        };

        /**
         * @brief Returns a thread local versoin of curve context at the given index.
         * @param idx The index of the context.
         * @return The curve context.
         */
        const CurveContext<adouble>& curveContext(size_t idx) const {
            if (idx < curveContexts_.size()) {
                return curveContexts_.at(idx);
            } else {
                std::string msg = "No curve context found at the given index: " + std::to_string(idx);
                throw std::out_of_range(msg);
            }
        };

        /**
         * @brief Returns the currency context for the given name.
         *
         * @param currency
         * @return const CurrencyContext<adouble>&
         */
        const CurrencyContext<adouble>& currencyContext(const std::string& contextName) const {
            if (ccyToIdx_.find(contextName) != ccyToIdx_.end()) {
                size_t pos = ccyToIdx_.at(contextName);
                return currencyContexts_.at(pos);
            } else {
                std::string msg = "No curve context found with the given name: " + contextName;
                throw std::out_of_range(msg);
            }
        };

        /**
         * @brief Return a currency context at the given index.
         *
         * @param idx
         * @return const CurrencyContext<adouble>&
         */
        const CurrencyContext<adouble>& currencyContext(size_t idx) const {
            if (idx < currencyContexts_.size()) {
                return currencyContexts_.at(idx);
            } else {
                std::string msg = "No curve context found at the given index: " + std::to_string(idx);
                throw std::out_of_range(msg);
            }
        };

        /**
         * @brief Checks if the store has a context with the given name.
         *
         * @param contextName string name of the context.
         * @return true if the store has a context with the given name.
         * @return false if the store does not have a context with the given name.
         */
        inline bool hasCurveContext(const std::string& contextName) const { return curveToIdx_.find(contextName) != curveToIdx_.end(); }

        /**
         * @brief clones from the given store. Cloning makes a deep copy of the store.
         *
         * @param store
         */
        void cloneFromStore(const MarketStore& store) {
            refDate_          = store.refDate_;
            localCcy_         = store.localCcy_;
            ccyToIdx_         = store.ccyToIdx_;
            currencyContexts_ = store.currencyContexts_;

            for (auto& context : store.curveContexts_) { curveContexts_.push_back(context.clone()); }
            curveToIdx_ = store.curveToIdx_;
        };

       private:
        Date refDate_;
        Currency localCcy_;

        std::map<std::string, size_t> ccyToIdx_;
        std::vector<CurrencyContext<adouble>> currencyContexts_;

        std::map<std::string, size_t> curveToIdx_;
        std::vector<CurveContext<adouble>> curveContexts_;
    };

    template <typename adouble>
    class MarketStore2 {
       public:
        MarketStore2(const Date& refDate, Currency localCcy = USD()) : refDate_(refDate), localCcy_(localCcy) {
            curveManager_     = std::make_unique<ContextManager<YieldTermStructure<adouble>>>(refDate_);
            rateIndexManager_ = std::make_unique<ContextManager<RateIndex>>(refDate_);
            fxManager_        = std::make_unique<ExchangeRateManager<adouble>>(refDate_);
        };

        void addCurve(const std::string& name, const YieldTermStructure<adouble>& curve, const RateIndex& index) {
            if (curveManager_->hasContext(name)) {
                throw std::invalid_argument("A curve context with the given name already exists.");
            } else {
                curveManager_->createContext(name, curve);
                rateIndexManager_->createContext(name, index);
            }
        };

        const YieldTermStructure<adouble>& curve(const std::string& name) const { return curveManager_->getContext(name).object(); };

        const YieldTermStructure<adouble>& curve(size_t idx) const { return curveManager_->getContext(idx).object(); };

        size_t curveIdx(const std::string& name) const { return curveManager_->getContext(name).idx(); };

        const RateIndex& rateIndex(const std::string& name) const { return rateIndexManager_->getContext(name).object(); };

        const RateIndex& rateIndex(size_t idx) const { return rateIndexManager_->getContext(idx).object(); };

        size_t rateIndexIdx(const std::string& name) const { return rateIndexManager_->getContext(name).idx(); };

        void addExchangeRate(const Currency& weak, const Currency& strong, adouble rate) {
            if (fxManager_->hasExchangeRate(weak, strong)) {
                throw std::invalid_argument("An exchange rate with the given currencies already exists.");
            } else {
                fxManager_->addExchangeRate(weak, strong, rate);
            }
        };

        adouble exchange(const Currency& weak, const Currency& strong) const { return fxManager_->exchange(weak, strong); };

        const Currency& localCcy() const { return localCcy_; };

       private:
        Date refDate_;
        Currency localCcy_;

        std::unique_ptr<ContextManager<YieldTermStructure<adouble>>> curveManager_;
        std::unique_ptr<ContextManager<RateIndex>> rateIndexManager_;
        std::unique_ptr<ExchangeRateManager<adouble>> fxManager_;
    };
}  // namespace Atlas

#endif /* C49C3B38_A428_4D92_AB3E_8E88957A544A */
