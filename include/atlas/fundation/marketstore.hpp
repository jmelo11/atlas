#ifndef C49C3B38_A428_4D92_AB3E_8E88957A544A
#define C49C3B38_A428_4D92_AB3E_8E88957A544A

#include <atlas/atlasconfig.hpp>
#include <atlas/rates/curvecontext.hpp>

namespace Atlas {

    template <typename adouble>
    using CcyRecepy = std::function<adouble(const Date&, const Date&, const MarketStore<adouble>&)>;

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
         * @param localCcy The local currency.
         */
        MarketStore(const Date& refDate = Date(), Currency localCcy = Currency()) : refDate_(refDate), localCcy_(localCcy) {
            // Create a default curve context for the local currency.
            adouble ccyValue = 1.0;
            createCurrency(localCcy_, ccyValue);
        }

        /**
         * @brief Creates a copy of curve context and adds it to the store.
         * @param contextName The name of the context.
         * @param curve The yield term structure.
         * @param index The rate index used as the underlying interest rate.
         */
        void createCurveContext(const std::string& contextName, const YieldTermStructure<adouble>& curve, const RateIndex& index) {
            if (ctxToIdx_.count(contextName) == 0) {
                auto curveClone = curve.clone();
                auto indexClone = index.clone();
                CurveContext<adouble> context(curveClone, indexClone, contexts_.size());
                contexts_.push_back(std::move(context));
                ctxToIdx_[contextName] = contexts_.size() - 1;
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
         * @return * void
         */

        void createCurrency(const Currency& currency, adouble value, CcyRecepy<adouble> ccyRecepy = nullptr) {
            if (currencies_.count(currency.numericCode()) == 0) {
                currencies_[currency.numericCode()] = {value, ccyRecepy};
            } else {
                throw std::invalid_argument("A currency context with the given code already exists.");
            }
        };

        /**
         * @brief Returns the thread local version of the curve context with the given name.
         * @param contextName The name of the context.
         * @return The curve context.
         */
        const CurveContext<adouble>& curveContext(const std::string& contextName) const {
            auto it = ctxToIdx_.find(contextName);
            if (it != ctxToIdx_.end()) {
                size_t pos = ctxToIdx_.at(contextName);
                return contexts_.at(pos);
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
            if (idx < contexts_.size()) {
                return contexts_[idx];
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
        inline bool hasCurveContext(const std::string& contextName) const { return ctxToIdx_.find(contextName) != ctxToIdx_.end(); }

        /**
         * @brief Returns a tuple of the value of the currency and the recepy to forecast the currency.
         *
         * @param currency
         * @return const std::tuple<adouble, CcyRecepy<adouble>>&
         */
        const std::tuple<adouble, CcyRecepy<adouble>>& currency(const Currency& currency) const {
            if (currencies_.find(currency.numericCode()) == currencies_.end()) {
                std::string msg = "Currency " + currency.name() + " was not found in the store.";
                throw std::out_of_range(msg);
            } else {
                return currencies_.at(currency.numericCode());
            }
        };

        /**
         * @brief Returns a tuple of the value of the currency and the recepy to forecast the currency.
         *
         * @param idx
         * @return const std::tuple<adouble, CcyRecepy<adouble>>&
         */
        const std::tuple<adouble, CcyRecepy<adouble>>& currency(size_t idx) const {
            if (currencies_.find(idx) == currencies_.end()) {
                std::string msg = "Currency with index " + std::to_string(idx) + " was not found in the store.";
                throw std::out_of_range(msg);
            } else {
                return currencies_.at(idx);
            }
        };

        /**
         * @brief clones from the given store. Cloning makes a deep copy of the store.
         *
         * @param store
         */
        void cloneFromStore(const MarketStore& store) {
            for (auto& context : store.contexts_) { contexts_.push_back(context.clone()); }
            ctxToIdx_   = store.ctxToIdx_;
            currencies_ = store.currencies_;
            localCcy_   = store.localCcy_;
            refDate_    = store.refDate_;
        };

       private:
        Date refDate_;
        Currency localCcy_;

        std::map<size_t, std::tuple<adouble, CcyRecepy<adouble>>> currencies_;
        std::map<std::string, size_t> ctxToIdx_;
        std::vector<CurveContext<adouble>> contexts_;
    };

}  // namespace Atlas

#endif /* C49C3B38_A428_4D92_AB3E_8E88957A544A */
