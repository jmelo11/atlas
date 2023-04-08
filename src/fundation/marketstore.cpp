#include <atlas/fundation/marketstore.hpp>

namespace Atlas {

    void MarketStore::createCurveContext(const std::string& contextName, const CurveStrategy& curve, const RateIndex& index) {
        if (nameToIdx_.count(contextName) == 0) {
            auto curveCopy = curve.copy();
            auto indexCopy = index.copy();
            CurveContext context(curveCopy, indexCopy, contexts_.size());
            contexts_.push_back(std::move(context));
            nameToIdx_[contextName] = contexts_.size() - 1;
        } else {
            throw std::invalid_argument("A curve context with the given name already exists.");
        }
    }

    const CurveContext& MarketStore::curveContext(const std::string& contextName) const {
        auto it = nameToIdx_.find(contextName);
        if (it != nameToIdx_.end()) {
            return contexts_[it->second];
        } else {
            throw std::out_of_range("No curve context found with the given name.");
        }
    }

    const CurveContext& MarketStore::curveContext(size_t idx) const {
        if (idx < contexts_.size()) {
            return contexts_[idx];
        } else {
            throw std::out_of_range("The index is out of the range of available curve contexts.");
        }
    }

    void MarketStore::copyFromStore(const MarketStore& store) {
        if (this == &store) {
            return;
        } else {
            contexts_.reserve(store.contexts_.size());
            for (const auto& [name, id] : store.nameToIdx_) {
                auto& context = store.contexts_[id];
                createCurveContext(name, *context.curveStrategy(), context.index());
            }
        }
    }

}  // namespace Atlas