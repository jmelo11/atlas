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

        virtual void simulate(const std::vector<Date>& evalDates, Scenario<adouble>& scenario) const = 0;

       protected:
        virtual void simulateDiscounts(const Date& evalDate, MarketData<adouble>& md) const = 0;

        virtual void simulateForwards(const Date& evalDate, MarketData<adouble>& md) const = 0;

        virtual void simulateSpots(const Date& evalDate, MarketData<adouble>& md) const = 0;

        MarketRequest marketRequest_;
    };

}  // namespace Atlas

#endif /* CB4131F2_7926_4625_AEEC_DD73178CBD54 */
