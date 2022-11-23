/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef CB4131F2_7926_4625_AEEC_DD73178CBD54
#define CB4131F2_7926_4625_AEEC_DD73178CBD54

#include <atlas/data/marketdata.hpp>

namespace Atlas {

    using Scenario = std::vector<MarketData>;

    class Model {
       public:
        virtual ~Model() = default;

        Model(const MarketRequest& marketRequest) : marketRequest_(marketRequest) {}

        virtual void simulate(const std::vector<QuantLib::Date>& evalDates, Scenario& scenario) const = 0;

       protected:
        MarketRequest marketRequest_;
    };

}  // namespace Atlas

#endif /* CB4131F2_7926_4625_AEEC_DD73178CBD54 */
