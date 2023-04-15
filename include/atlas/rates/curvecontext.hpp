#ifndef C9383CFF_FA55_4061_A633_C8B6F16B0878
#define C9383CFF_FA55_4061_A633_C8B6F16B0878

#include <atlas/atlasconfig.hpp>
#include <atlas/rates/rateindex.hpp>
#include <atlas/rates/yieldtermstructure/yieldtermstructure.hpp>

namespace Atlas {
    template <typename adouble>
    class MarketStore;

    /**
     * @class CurveContext
     * @brief A context for a yield curve. Contains the yield term structure and the rate index used as the underlying interest rate. It contains its
     * own copy of the curve and index.
     */
    template <typename adouble>
    class CurveContext {
       public:
        /**
         * @return The yield term structure.
         */
        inline const YieldTermStructure<adouble>& curve() const { return *curve_; }

        /**
         * @return The rate index used as the underlying interest rate.
         */
        inline const RateIndex& index() const { return *index_; }

        /**
         * @return The index of the context in the store.
         */
        inline size_t idx() const { return idx_; }

       private:
        /**
         * Constructs a yield curve from a vector of dates and a vector of corresponding rates.
         * @param curve unique_ptr to the CurveStrategy
         * @param index unique_ptr to the RateIndex
         * @param idx positional index in the CurveContextStore
         */
        CurveContext(std::unique_ptr<YieldTermStructure<adouble>>& curve, std::unique_ptr<RateIndex>& index, size_t idx)
        : curve_(std::move(curve)), index_(std::move(index)), idx_(idx) {}

        CurveContext clone() const {
            auto curveCopy = curve_->clone();
            auto indexCopy = index_->clone();
            return CurveContext(curveCopy, indexCopy, idx_);
        }

        std::unique_ptr<YieldTermStructure<adouble>> curve_;
        std::unique_ptr<RateIndex> index_;
        size_t idx_;
        friend class MarketStore<adouble>;
    };

}  // namespace Atlas

#endif /* C9383CFF_FA55_4061_A633_C8B6F16B0878 */
