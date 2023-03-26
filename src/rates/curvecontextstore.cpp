#include <atlas/rates/curvecontextstore.hpp>

namespace Atlas {

    thread_local std::unique_ptr<CurveContextStore> CurveContextStore::instance_         = nullptr;
    thread_local std::map<std::string, size_t> CurveContextStore::nameToIdx_             = {};
    thread_local std::vector<std::unique_ptr<CurveContext>> CurveContextStore::contexts_ = {};

    CurveContextStore& CurveContextStore::instance() {
        if (instance_ == nullptr) { instance_ = std::unique_ptr<CurveContextStore>(new CurveContextStore()); }
        return *instance_;
    }

    void CurveContextStore::createCurveContext(const std::string& contextName, const CurveStrategy& curve, const RateIndex& index) {
        if (nameToIdx_.count(contextName) == 0) {
            std::unique_ptr<CurveContext> context(new CurveContext(curve.copy(), index.copy(), contexts_.size()));
            contexts_.push_back(std::move(context));
            nameToIdx_[contextName] = contexts_.size() - 1;
        } else {
            throw std::invalid_argument("A curve context with the given name already exists.");
        }
    }

    const CurveContext& CurveContextStore::at(const std::string& contextName) const {
        auto it = nameToIdx_.find(contextName);
        if (it != nameToIdx_.end()) {
            return *contexts_[it->second];
        } else {
            throw std::out_of_range("No curve context found with the given name.");
        }
    }

    const CurveContext& CurveContextStore::at(size_t idx) const {
        if (idx < contexts_.size()) {
            return *contexts_[idx];
        } else {
            throw std::out_of_range("The index is out of the range of available curve contexts.");
        }
    }

}  // namespace Atlas