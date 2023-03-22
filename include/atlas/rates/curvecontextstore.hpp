#ifndef C49C3B38_A428_4D92_AB3E_8E88957A544A
#define C49C3B38_A428_4D92_AB3E_8E88957A544A

#include <atlas/atlasconfig.hpp>
#include <atlas/rates/curvecontext.hpp>

namespace Atlas {

    /**
     * @class CurveContextStore
     * @brief A store for curve contexts.
     */
    class CurveContextStore {
       public:
        CurveContextStore() = default;

        /***
         * @brief Creates a curve context and adds it to the store.
         * @param contextName The name of the context.
         * @param curve The yield term structure.
         * @param index The rate index used as the underlying interest rate.
         */
        void createCurveContext(const std::string& contextName, std::unique_ptr<QuantLib::YieldTermStructure> curve,
                                std::unique_ptr<RateIndex> index);

        /***
         * @brief Returns the curve context with the given name.
         * @param contextName The name of the context.
         * @return The curve context.
         */

        std::shared_ptr<CurveContext> at(const std::string& contextName) const;

        /***
         * @brief Returns the curve context at the given index.
         * @param idx The index of the context.
         * @return The curve context.
         */
        std::shared_ptr<CurveContext> at(size_t idx) const;

       private:
        std::map<std::string, size_t> nameToIdx_;
        std::vector<std::shared_ptr<CurveContext>> contexts_;
    };

}  // namespace Atlas

#endif /* C49C3B38_A428_4D92_AB3E_8E88957A544A */
