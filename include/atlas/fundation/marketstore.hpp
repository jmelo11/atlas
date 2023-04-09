#ifndef C49C3B38_A428_4D92_AB3E_8E88957A544A
#define C49C3B38_A428_4D92_AB3E_8E88957A544A

#include <atlas/atlasconfig.hpp>
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
         * @return The instance of the store.
         */
        MarketStore() = default;

        /**
         * @brief Creates a copy of curve context and adds it to the store.
         * @param contextName The name of the context.
         * @param curve The yield term structure.
         * @param index The rate index used as the underlying interest rate.
         */
        void createCurveContext(const std::string& contextName, const YieldTermStructure<adouble>& curve, const RateIndex& index) {
            if (nameToIdx_.count(contextName) == 0) {
                auto curveCopy = curve.copy();
                auto indexCopy = index.copy();
                CurveContext<adouble> context(curveCopy, indexCopy, contexts_.size());
                contexts_.push_back(std::move(context));
                nameToIdx_[contextName] = contexts_.size() - 1;
            } else {
                throw std::invalid_argument("A curve context with the given name already exists.");
            }
        };

        /**
         * @brief Returns the thread local version of the curve context with the given name.
         * @param contextName The name of the context.
         * @return The curve context.
         */
        CurveContext<adouble>& curveContext(const std::string& contextName) {
            auto it = nameToIdx_.find(contextName);
            if (it != nameToIdx_.end()) {
                return contexts_.at(it->second);
            } else {
                throw std::out_of_range("No curve context found with the given name.");
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
                throw std::out_of_range("The index is out of the range of available curve contexts.");
            }
        };
        /**
         * @brief Checks if the store has a context with the given name.
         *
         * @param contextName string name of the context.
         * @return true if the store has a context with the given name.
         * @return false if the store does not have a context with the given name.
         */
        inline bool hasCurveContext(const std::string& contextName) const { return nameToIdx_.find(contextName) != nameToIdx_.end(); }

        void copyFromStore(const MarketStore& store)  {
            for (auto& context : store.contexts_) {
                contexts_.push_back(context.copy());
            }
            nameToIdx_ = store.nameToIdx_;
        };

       private:
        Date referenceDate_ = Date();
        std::map<std::string, size_t> nameToIdx_;
        std::vector<CurveContext<adouble>> contexts_;
    };

}  // namespace Atlas

#endif /* C49C3B38_A428_4D92_AB3E_8E88957A544A */
