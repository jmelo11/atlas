#include <atlas/rates/curvecontextstore.hpp>

namespace Atlas {

    void CurveContextStore::createCurveContext(const std::string& contextName, std::unique_ptr<QuantLib::YieldTermStructure> curve,
                                               std::unique_ptr<RateIndex> index) {
        nameToIdx_[contextName] = contexts_.size();
        std::shared_ptr<CurveContext> context(new CurveContext(std::move(curve), std::move(index), contexts_.size()));
        contexts_.push_back(std::move(context));
    };

    std::shared_ptr<CurveContext> CurveContextStore::at(const std::string& contextName) const {
        return contexts_.at(nameToIdx_.at(contextName));
    };

    std::shared_ptr<CurveContext> CurveContextStore::at(size_t idx) const {
        return contexts_.at(idx);
    };

}  // namespace Atlas