#ifndef C9383CFF_FA55_4061_A633_C8B6F16B0878
#define C9383CFF_FA55_4061_A633_C8B6F16B0878

#include <atlas/atlasconfig.hpp>
#include <atlas/rates/rateindex.hpp>

namespace Atlas {
    /**
     * @class CurveContext
     * @brief A context for a yield curve. Contains the yield term structure and the rate index used as the underlying interest rate.
     */
    class CurveContext {
       public:
        /**
         * @return The yield term structure.
         */
        const QuantLib::YieldTermStructure& curve() const { return *curve_; }

        /**
         * @return The rate index used as the underlying interest rate.
         */
        const RateIndex& index() const { return *index_; }

        /**
         * @return The index of the context in the store.
         */
        size_t idx() const { return idx_; }

       private:
        /**
         * Constructs a yield curve from a vector of dates and a vector of corresponding rates.
         * @param curve The yield term structure.
         * @param index The rate index used as the underlying interest rate.
         */
        CurveContext(std::unique_ptr<QuantLib::YieldTermStructure> curve, std::unique_ptr<RateIndex> index, size_t idx)
        : curve_(std::move(curve)), index_(std::move(index)), idx_(idx) {}

        std::unique_ptr<QuantLib::YieldTermStructure> curve_; /**< The yield term structure. */
        std::unique_ptr<RateIndex> index_;                    /**< The rate index used as the underlying interest rate. */
        size_t idx_;                                          /**< The index of the context in the store. */
        friend class CurveContextStore;
    };

}  // namespace Atlas

#endif /* C9383CFF_FA55_4061_A633_C8B6F16B0878 */
