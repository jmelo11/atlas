#ifndef C9383CFF_FA55_4061_A633_C8B6F16B0878
#define C9383CFF_FA55_4061_A633_C8B6F16B0878

#include <atlas/atlasconfig.hpp>
#include <atlas/rates/curvestrategy.hpp>
#include <atlas/rates/rateindex.hpp>

namespace Atlas {
    /**
     * @class CurveContext
     * @brief A context for a yield curve. Contains the yield term structure and the rate index used as the underlying interest rate. It contains its
     * own copy of the curve and index.
     */
    class CurveContext {
       public:
        /**
         * @return The yield term structure.
         */
        inline const QuantLib::YieldTermStructure& curve() const { return curveStrategy_->curve(); }

        /**
         * @return The rate index used as the underlying interest rate.
         */
        inline const RateIndex& index() const { return *index_; }

        /**
         * @return The index of the context in the store.
         */
        inline size_t idx() const { return idx_; }

        inline const std::unique_ptr<CurveStrategy>& curveStrategy() const { return curveStrategy_; }
       private:
        /**
         * Constructs a yield curve from a vector of dates and a vector of corresponding rates.
         * @param curve unique_ptr to the CurveStrategy
         * @param index unique_ptr to the RateIndex
         * @param idx positional index in the CurveContextStore
         */
        CurveContext(std::unique_ptr<CurveStrategy>& curveStrategy, std::unique_ptr<RateIndex>& index, size_t idx)
        : curveStrategy_(std::move(curveStrategy)), index_(std::move(index)), idx_(idx) {}

        std::unique_ptr<CurveStrategy> curveStrategy_;
        std::unique_ptr<RateIndex> index_;
        size_t idx_;
        friend class MarketStore;
    };

}  // namespace Atlas

#endif /* C9383CFF_FA55_4061_A633_C8B6F16B0878 */
