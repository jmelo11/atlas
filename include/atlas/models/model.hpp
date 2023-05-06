/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef CB4131F2_7926_4625_AEEC_DD73178CBD54
#define CB4131F2_7926_4625_AEEC_DD73178CBD54

#include <atlas/atlasconfig.hpp>
#include <atlas/data/marketdata.hpp>

namespace Atlas {

    template <typename adouble>
    using Scenario = std::vector<MarketData<adouble>>;

    /**
     * @brief Model interface
     *
     * @tparam adouble
     */
    template <typename adouble>
    class Model {
       public:
        virtual ~Model() = default;

        Model(const MarketRequest& marketRequest) : marketRequest_(marketRequest) {}

        virtual MarketData<adouble> marketData(const Date& evalDate = Date()) const = 0;

        MarketRequest marketRequest_;
    };

    template <typename adouble>
    class MonteCarloEngine {
        virtual bool nextScenario(const std::vector<Date>& evalDates, Scenario<adouble>& scenario) const = 0;
    };

}  // namespace Atlas

#endif /* CB4131F2_7926_4625_AEEC_DD73178CBD54 */
